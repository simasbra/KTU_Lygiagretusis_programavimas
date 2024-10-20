#include <cstring>
#include <ctime>
#include <omp.h>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "openmpMonitor.h"
#include <unistd.h>

using namespace std;

const string FILE_PATH_RESULT = "results/IFF22_BradaitisV_L1_rez.txt";

void read_file(const string &filePath, rapidjson::Document *pDocument);
User get_user_from_value(rapidjson::Value &userValue);
vector<vector<User>> split_users_to_equal_parts(rapidjson::Value *pUsersArray, const int &MAX_THREAD_COUNT, const int &USER_COUNT);

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
		printf("Users: \n");
		printf(" %3s | %-20s | %-10s | %-10s |\n", "No.", "Name", "Year", "Day Month");
		for (unsigned int i = 0; i < usersArray.Size(); i++) {
			User userTemporary = get_user_from_value(usersArray[i]);
			printf("%4d ", i + 1);
			userTemporary.print_user();
		}
	} else return 1;

	clock_t clockBegin = clock();
	const unsigned int USER_COUNT = usersArray.Size();
	const int MAX_THREAD_COUNT = max(1, (int)min((int)(USER_COUNT / 4), (int)sysconf(_SC_NPROCESSORS_ONLN)));

	vector<vector<User>> users = split_users_to_equal_parts(&usersArray, MAX_THREAD_COUNT, USER_COUNT);
	OpenMPMonitor *pOpenMPMonitor = new OpenMPMonitor();
	omp_set_num_threads(MAX_THREAD_COUNT);
	int sumInt = 0;
	float sumFloat = 0;
	#pragma omp parallel default(none) shared(users, pOpenMPMonitor) reduction(+:sumInt, sumFloat)
	{
		const int THREAD_NUM = omp_get_thread_num();
		vector<User> usersTemporary = users[THREAD_NUM];
		printf("Processing thread %d with %lu users\n", THREAD_NUM, usersTemporary.size());
		for (unsigned int i = 0; i < usersTemporary.size(); i++) {
			User userTemporary = usersTemporary[i];
			if (!userTemporary.is_valid()) {
				continue;
			}
			if (pOpenMPMonitor->process_user_result(&userTemporary)) {
				sumInt += userTemporary.get_year();
				sumFloat += userTemporary.get_day_month();
			}
		}
	}
	pOpenMPMonitor->set_sum_int(sumInt);
	pOpenMPMonitor->set_sum_float(sumFloat);

	clock_t clockEnd = clock();
	double timeSpent = (double)(clockEnd - clockBegin) / CLOCKS_PER_SEC;
	pOpenMPMonitor->print_users_result();
	pOpenMPMonitor->print_users_result_to_file(FILE_PATH_RESULT);
	printf("Time spent: %lfs\n", timeSpent);

	delete pOpenMPMonitor;
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

vector<vector<User>> split_users_to_equal_parts(rapidjson::Value *pUsersArray, const int &MAX_THREAD_COUNT, const int &USER_COUNT) {
	vector<vector<User>> users(MAX_THREAD_COUNT);
	int usersAddedToArrays = 0;
	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int usersCount = round((USER_COUNT - usersAddedToArrays) / (MAX_THREAD_COUNT - i));
		for (int j = 0; j < usersCount; j++) {
			User userTemporary = get_user_from_value((*pUsersArray)[usersAddedToArrays + j]);
			users[i].push_back(userTemporary);
		}
		usersAddedToArrays += usersCount;
	}
	return users;
}
