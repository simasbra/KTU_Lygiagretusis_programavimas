#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"
#include "usersMonitor.h"
#include "user.h"

using namespace std;

UsersMonitor::UsersMonitor() : currentSize_(0) {}

UsersMonitor::~UsersMonitor() {}

void UsersMonitor::print_users() {
	for (int i = 0; i < currentSize_; i++) {
		printf("%4d ", i + 1);
		User::print_user(users_[i]);
	}
}

int UsersMonitor::get_current_size() {
	return currentSize_;
}

int UsersMonitor::get_users_read() {
	return usersRead_;
}

void UsersMonitor::add_user_last(User userNew) {
	if (currentSize_ >= MAX_SIZE_) {
		return;
	}

	users_[currentSize_] = userNew;
	currentSize_++;
}

void UsersMonitor::add_user_sorted(User userNew) {
	if (currentSize_ == 0) {
		add_user_last(userNew);
		return;
	}
	if (currentSize_ >= MAX_SIZE_) {
		return;
	}

	int i;
	for (i = currentSize_ - 1; i >= 0; i--) {
		if (users_[i].get_year() < userNew.get_year()) {
			break;
		}
		users_[i + 1] = users_[i];
	}
	users_[i + 1] = userNew;
	currentSize_++;
}

User UsersMonitor::remove_user_last() {
	if (currentSize_ <= 0) {
		return User();
	}

	User userTemporary = users_[--currentSize_];
	users_[currentSize_] = User();
	return userTemporary;
}

int UsersMonitor::get_file_user_count(const std::string &filePath) {
	FILE *pFile = fopen(filePath.c_str(), "r");
	if (pFile == NULL) {
		perror("Failed to open a file\n");
		return 0;
	}

	char readBuffer[65536];
	rapidjson::FileReadStream jsonStream(pFile, readBuffer, sizeof(readBuffer));
	rapidjson::Document jsonDocument;
	jsonDocument.ParseStream(jsonStream);
	fclose(pFile);

	if (jsonDocument.HasParseError()) {
		perror("Failed to parse into Document\n");
		return 0;
	}
	if (jsonDocument.HasMember("usersCount") && jsonDocument["usersCount"].IsInt()) {
		return jsonDocument["usersCount"].GetInt();
	}
	return 0;
}

void UsersMonitor::read_file(const string &filePath) {
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
		perror("Failed to parse into Document\n");
		return;
	}
	if (jsonDocument.HasMember("users") && jsonDocument["users"].IsArray()) {
		const rapidjson::Value &usersArray = jsonDocument["users"];

		for (rapidjson::SizeType i = usersRead_; currentSize_ < MAX_SIZE_ && usersRead_ < (int)usersArray.Size(); ++i) {
			const rapidjson::Value &userCurrent = usersArray[i];
			if (userCurrent.HasMember("name") && userCurrent["name"].IsString() && userCurrent.HasMember("year") && userCurrent["year"].IsInt() && userCurrent.HasMember("dayMonth") && userCurrent["dayMonth"].IsDouble()) {
				const string name = userCurrent["name"].GetString();
				const int year = userCurrent["year"].GetInt();
				const double dayMonth = userCurrent["dayMonth"].GetDouble();
				User userToAdd = User(name, year, dayMonth);
				add_user_last(userToAdd);
				usersRead_++;
			}
		}
	}
}
