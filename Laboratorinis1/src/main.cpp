#include <cstring>
#include <ctime>
#include <pthread.h>
#include <thread>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "usersMonitor.h"
#include "usersResultMonitor.h"

using namespace std;

const string FILE_PATH_RESULT = "results/IFF22_BradaitisV_L1_rez.txt";

void read_file(const string &filePath, rapidjson::Document *pDocument);
User get_user_from_value(rapidjson::Value &userValue);
void print_monitors_statistics(UsersMonitor *pUsersMonitor, UsersResultMonitor *pUsersResultMonitor);
void *create_thread(void *arg);

int main(int args, char *arg[]) {
	if (args != 2) {
		printf("Usage: %s <filepath>\n", arg[0]);
		return 1;
	}
	const char *FILE_PATH_DATA = arg[1];

	rapidjson::Document jsonDocument;
	read_file(FILE_PATH_DATA, &jsonDocument);
	rapidjson::Value usersArray;

	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		usersArray = jsonDocument["users"];
		/*printf("Users: \n");*/
		/*for (unsigned int i = 0; i < usersArray.Size(); i++) {*/
		/*	User userTemporary = get_user_from_value(usersArray[i]);*/
		/*	userTemporary.print_user();*/
		/*}*/
	} else return 1;

	clock_t clockBegin = clock();
	const unsigned int USER_COUNT = usersArray.Size();
	const int MAX_THREAD_COUNT = min(USER_COUNT / 4, thread::hardware_concurrency());
	pthread_t threads[MAX_THREAD_COUNT];

	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(USER_COUNT, pUsersMonitor);

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_create(&threads[i], NULL, create_thread, pUsersResultMonitor);
		if (error != 0) {
			printf("Thread cannot be created: [%s]", strerror(error));
		}
	}

	int i = 0;
	while (pUsersMonitor->get_users_added() < USER_COUNT) {
		pUsersMonitor->add_user_last(get_user_from_value(usersArray[i]));
		i++;
	}

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_join(threads[i], NULL);
		if (error != 0) {
			printf("Thread cannot be joined: [%s]", strerror(error));
		}
	}

	clock_t clockEnd = clock();
	double timeSpent = (double)(clockEnd - clockBegin) / CLOCKS_PER_SEC;
	print_monitors_statistics(pUsersMonitor, pUsersResultMonitor);
	pUsersResultMonitor->print_users_result_to_file(FILE_PATH_RESULT);
	printf("Time spent: %lfs\n", timeSpent);

	delete pUsersMonitor;
	delete pUsersResultMonitor;
	return 0;
}

void read_file(const string &filePath, rapidjson::Document *pDocument) {
	FILE *pFile = fopen(filePath.c_str(), "r");
	if (pFile == NULL) {
		perror("Failed to open a file\n");
		return;
	}

	char readBuffer[65536];
	rapidjson::FileReadStream jsonStream(pFile, readBuffer, sizeof(readBuffer));
	pDocument->ParseStream(jsonStream);
	fclose(pFile);

	if (pDocument->HasParseError()) {
		perror("Failed to parse into Document\n");
		return;
	}
}

void print_monitors_statistics(UsersMonitor *pUsersMonitor, UsersResultMonitor *pUsersResultMonitor) {
	printf("\nUser Monitor: \n");
	pUsersMonitor->print_users();
	printf("Users added count: %d\n", pUsersMonitor->get_users_added());
	printf("User Monitor count: %d\n", pUsersMonitor->get_current_size());
	printf("\nUser Result Monitor: \n");
	pUsersResultMonitor->print_users_result();
	printf("Users processed count: %d\n", pUsersResultMonitor->get_users_processed());
	printf("User Result Monitor count: %d\n", pUsersResultMonitor->get_current_size());
}

void *create_thread(void *arg) {
	UsersResultMonitor *pUsersResultMonitor = (UsersResultMonitor *) arg;
	while (!pUsersResultMonitor->check_all_users_processed()) {
		User userTemporary = pUsersResultMonitor->get_user_last_from_users_monitor();
		pUsersResultMonitor->process_user_result(&userTemporary);
	}
	return NULL;
}
