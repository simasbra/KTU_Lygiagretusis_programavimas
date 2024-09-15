#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "usersMonitor.h"
#include "usersResultMonitor.h"

using namespace std;

const string filePathData = "data/IFF22_BradaitisV_L1_dat_1.json";
const string filePathResult = "results/IFF22_BradaitisV_L1_rez.txt";

void read_file(const string &filePath, rapidjson::Document *pDocument);
User get_user_from_value(rapidjson::Value &userValue);
void print_monitors_statistics(UsersMonitor *pUsersMonitor, UsersResultMonitor *pUsersResultMonitor);

int main(void) {
	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(pUsersMonitor);

	rapidjson::Document jsonDocument;
	read_file(filePathData, &jsonDocument);
	rapidjson::Value usersArray;
	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		usersArray = jsonDocument["users"];
	}

	const int USER_COUNT = usersArray.Size();
	/*const int MAX_THREAD_COUNT = USER_COUNT / 4;*/
	/*int currentThreadCount = 0;*/

	for (int i = 0; i < USER_COUNT; i++) {
		User userToAdd = get_user_from_value(usersArray[i]);
		pUsersMonitor->add_user_last(userToAdd);
		pUsersResultMonitor->steal_generate_set_check_add_user_result();
		printf("Iteration: %4d Added: %4d Processed: %4d\n", i, pUsersMonitor->get_users_added(), pUsersResultMonitor->get_users_processed());
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
