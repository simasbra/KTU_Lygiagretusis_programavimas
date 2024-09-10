#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"
#include "usersMonitor.h"
#include "user.h"

using namespace std;

UsersMonitor::UsersMonitor() : currentSize(0) {}

UsersMonitor::~UsersMonitor() {}

void UsersMonitor::print_users() {
	for (int i = 0; i < currentSize; i++) {
		User::print_user(users[i]);
	}
}

void UsersMonitor::add_user(User user) {
	if (currentSize < maxSize) {
		users[currentSize] = user;
		currentSize++;
	}
}
/*void UsersMonitor::add_user_sorted(User user) {*/
/*	// will implement later*/
/*}*/
/*User UsersMonitor::remove_user_last() {*/
/*	// will implement later*/
/*}*/

void UsersMonitor::read_file(string filePath) {
	FILE *pFile = fopen(filePath.c_str(), "r");
	if (pFile == NULL) {
		perror("Failed to open a file\n");
		return;
	}

	char readBuffer[65536];
	rapidjson::FileReadStream jsonStream(pFile, readBuffer, sizeof(readBuffer));
	rapidjson::Document jsonDocument;
	jsonDocument.ParseStream(jsonStream);
	fclose(pFile);

	if (jsonDocument.HasParseError()) {
		perror("Failet to parse into Document\n");
		return;
	}

	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		const rapidjson::Value &usersArray = jsonDocument["users"];

		for (rapidjson::SizeType i = 0; i < usersArray.Size(); ++i) {
			const rapidjson::Value &userCurrent = usersArray[i];

			if (userCurrent.HasMember("name") && userCurrent["name"].IsString()
				&& userCurrent.HasMember("year") && userCurrent["year"].IsInt()
				&& userCurrent.HasMember("dayMonth") && userCurrent["dayMonth"].IsDouble()) {
				const string name = userCurrent["name"].GetString();
				const int year = userCurrent["year"].GetInt();
				const double dayMonth = userCurrent["dayMonth"].GetDouble();
				User userToAdd = User(name, year, dayMonth);
				add_user(userToAdd);
			}
		}
	}
}
