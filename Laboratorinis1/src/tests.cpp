#include <cstring>
#include <ctime>
#include <pthread.h>
#include <thread>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "userResult.h"
#include "usersMonitor.h"
#include "usersResultMonitor.h"

using namespace std;

typedef enum AlgorithmType {
	SHA256,
	BLAKE2B,
	COUNT,
} AlgorithmType;

struct ThreadArguments {
	UsersResultMonitor *pUsersMonitor;
	AlgorithmType *pType;
};

const string ALGORITHM_TYPES[] = {
	"SHA256",
	"BLAKE2B"
};

void read_file(const string &filePath, rapidjson::Document *pDocument);
User get_user_from_value(rapidjson::Value &userValue);
void *create_thread(void *arg);
double run_test(rapidjson::Value &usersArray, AlgorithmType type, int testCount);

int main(int args, char *arg[]) {
	if (args != 3) {
		printf("Usage: %s <filepath>\n", arg[0]);
		return 1;
	}
	const char *FILE_PATH_DATA = arg[1];
	const int TEST_COUNT = atof(arg[2]);
	double testTimes[COUNT - 1];

	clock_t clockBegin = clock();
	rapidjson::Document jsonDocument;
	read_file(FILE_PATH_DATA, &jsonDocument);
	rapidjson::Value usersArray;

	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		usersArray = jsonDocument["users"];
	}
	else return 1;

	for (int i = 0; i < COUNT; i++) {
		testTimes[i] = run_test(usersArray, (AlgorithmType)i, TEST_COUNT);
	}
	clock_t clockEnd = clock();
	double timeSpent = (double)(clockEnd - clockBegin) / CLOCKS_PER_SEC;

	for (int i = 0; i < COUNT; i++) {
		printf("%s Time spent: %lfs\n", ALGORITHM_TYPES[i].c_str(), testTimes[i]);
	}
	printf("Time spent running tests: %lfs\n", timeSpent);

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
	struct ThreadArguments *pThreadArguments = (ThreadArguments *) arg;
	UsersResultMonitor *pUsersResultMonitor = pThreadArguments->pUsersMonitor;
	while (!pUsersResultMonitor->check_all_users_processed()) {
		User userTemporary = pUsersResultMonitor->get_user_last_from_users_monitor();
		if (userTemporary.is_valid()) {
			UserResult *pUserResultTemporary = new UserResult(userTemporary);
			switch(*(pThreadArguments->pType)) {
				case SHA256:
					pUserResultTemporary->hash_using_sha256();
					break;
				case BLAKE2B:
					pUserResultTemporary->hash_using_blake2b();
					break;
				default:
					break;
			}
			pUsersResultMonitor->increase_users_processed();
			if (!pUserResultTemporary->check_hash_ends_with_a_number()) {
				pUsersResultMonitor->add_user_result_sorted(*pUserResultTemporary);
			}
			delete pUserResultTemporary;
		}
	}
	return NULL;
}

double run_test(rapidjson::Value &usersArray, AlgorithmType type, int testCount) {
	double timeSpentAverage = 0;
	printf("\nAlgorithm: %s\n\n", ALGORITHM_TYPES[type].c_str());
	for (int i = 0; i < testCount; i++) {
		clock_t clockBegin = clock();
		const unsigned int USER_COUNT = usersArray.Size();
		const int MAX_THREAD_COUNT = min(USER_COUNT / 4, thread::hardware_concurrency());
		pthread_t threads[MAX_THREAD_COUNT];

		UsersMonitor *pUsersMonitor = new UsersMonitor();
		UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(USER_COUNT, pUsersMonitor);
		struct ThreadArguments threadArguments = { pUsersResultMonitor, &type };

		for (int j = 0; j < MAX_THREAD_COUNT; j++) {
			int error = pthread_create(&threads[j], NULL, create_thread, &threadArguments);
			if (error != 0) {
				printf("Thread cannot be created: [%s]", strerror(error));
			}
		}

		int j = 0;
		while (pUsersMonitor->get_users_added() < USER_COUNT) {
			pUsersMonitor->add_user_last(get_user_from_value(usersArray[j]));
			j++;
		}

		for (int j = 0; j < MAX_THREAD_COUNT; j++) {
			int error = pthread_join(threads[j], NULL);
			if (error != 0) {
				printf("Thread cannot be joined: [%s]", strerror(error));
			}
		}

		clock_t clockEnd = clock();
		double timeSpent = (double)(clockEnd - clockBegin) / CLOCKS_PER_SEC;
		timeSpentAverage += timeSpent;
		printf("%8d Time spent: %lfs\n", i, timeSpent);

		delete pUsersMonitor;
		delete pUsersResultMonitor;
	}
	timeSpentAverage /= testCount;
	return timeSpentAverage;
}
