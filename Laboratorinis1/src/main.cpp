#include <cstring>
#include <pthread.h>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "userResult.h"
#include "usersMonitor.h"
#include "usersResultMonitor.h"

using namespace std;

const string filePathData = "data/IFF22_BradaitisV_L1_dat_1.json";
const string filePathResult = "results/IFF22_BradaitisV_L1_rez.txt";

pthread_mutex_t mutex;
pthread_cond_t newUserAdded;

void read_file(const string &filePath, rapidjson::Document *pDocument);
User get_user_from_value(rapidjson::Value &userValue);
void print_monitors_statistics(UsersMonitor *pUsersMonitor, UsersResultMonitor *pUsersResultMonitor);
int initialize_mutex_and_cond();
void *create_thread(void *arg);

int main(void) {
	rapidjson::Document jsonDocument;
	read_file(filePathData, &jsonDocument);
	rapidjson::Value usersArray;
	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		usersArray = jsonDocument["users"];
	}

	const int USER_COUNT = usersArray.Size();
	const int MAX_THREAD_COUNT = USER_COUNT / 4;
	pthread_t threads[MAX_THREAD_COUNT];
	if (int error = initialize_mutex_and_cond() != 0) {
		return error;
	}

	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(USER_COUNT, pUsersMonitor);

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_create(&threads[i], NULL, create_thread, pUsersResultMonitor);
		if (error != 0) {
			printf("Thread cannot be created: [%s]", strerror(error));
		}
	}

	int i = 0, iteration = 0;
	while (pUsersMonitor->get_users_added() < USER_COUNT) {
		if (pUsersMonitor->get_current_size() < pUsersMonitor->get_max_size() && pUsersMonitor->get_users_added() < USER_COUNT) {
			User userToAdd = get_user_from_value(usersArray[i]);
			pthread_mutex_lock(&mutex);
			pUsersMonitor->add_user_last(userToAdd);
			pthread_cond_signal(&newUserAdded);
			pthread_mutex_unlock(&mutex);
			i++;
		}
		iteration++;
		printf("Iteration: %4d Added: %4d Processed: %4d\n", iteration, pUsersMonitor->get_users_added(), pUsersResultMonitor->get_users_processed());
	}

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_join(threads[i], NULL);
		if (error != 0) {
			printf("Thread cannot be joined: [%s]", strerror(error));
		}
	}

	/*print_monitors_statistics(pUsersMonitor, pUsersResultMonitor);*/
	pUsersResultMonitor->print_users_result_to_file(filePathResult);

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

User get_user_from_value(rapidjson::Value &userValue) {
	if (userValue.HasMember("name") && userValue["name"].IsString() && userValue.HasMember("year") && userValue["year"].IsInt() && userValue.HasMember("dayMonth") && userValue["dayMonth"].IsDouble()) {
		const string name = userValue["name"].GetString();
		const int year = userValue["year"].GetInt();
		const double dayMonth = userValue["dayMonth"].GetDouble();
		return User(name, year, dayMonth);
	}
	return User();
}

void print_monitors_statistics(UsersMonitor *pUsersMonitor, UsersResultMonitor *pUsersResultMonitor) {
	printf("\nUser Monitor: \n");
	pUsersMonitor->print_users();
	printf("User Monitor count: %d\n", pUsersMonitor->get_current_size());
	printf("\nUser Result Monitor: \n");
	pUsersResultMonitor->print_users_result();
	printf("User Result Monitor count: %d\n", pUsersResultMonitor->get_current_size());
}

int initialize_mutex_and_cond() {
	int error = 0;
	if ((error = pthread_mutex_init(&mutex, NULL)) != 0) {
		printf("Thread cannot be created: [%s]", strerror(error));
		return error;
	}
	if ((error = pthread_cond_init(&newUserAdded, NULL)) != 0) {
		printf("Thread cannot be created: [%s]", strerror(error));
		return error;
	}
	return 0;
}

void *create_thread(void *arg) {
	UsersResultMonitor *pUsersResultMonitor = (UsersResultMonitor *) arg;

	while (!pUsersResultMonitor->check_all_users_processed()) {
		pthread_mutex_lock(&mutex);
		User userTemporary = pUsersResultMonitor->get_user_last_from_users_monitor();
		pthread_mutex_unlock(&mutex);
		if (!userTemporary.is_valid()) {
			continue;
		}

		UserResult *pUserResultTemporary = new UserResult(userTemporary);
		pUserResultTemporary->set_hash(pUserResultTemporary->generate_sha256());

		pthread_mutex_lock(&mutex);
		if (!pUserResultTemporary->check_hash_ends_with_a_number()) {
			pUsersResultMonitor->add_user_result_sorted(*pUserResultTemporary);
		}
		pUsersResultMonitor->increase_users_processed();
		pthread_mutex_unlock(&mutex);

		UserResult::print_user_result(*pUserResultTemporary);
		delete pUserResultTemporary;
	}
	return NULL;
}
