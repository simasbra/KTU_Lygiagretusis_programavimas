#include "user.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"
#include <cstdio>

using namespace std;

User::User() : name(""), year(0), dayMonth(0) {}

User::User(string name, int year, double dayMonth) : name(name), year(year), dayMonth(dayMonth) {}

User::~User() {}

string User::get_name() {
	return name;
}

int User::get_year() {
	return year;
}

double User::get_day_month() {
	return dayMonth;
}

void User::set_name(string name) {
	this->name = name;
}

void User::set_year(int year) {
	this->year = year;
}

void User::set_day_month(double dayMonth) {
	this->dayMonth = dayMonth;
}

void User::print_user(User *pUser) {
	printf("Name: %s, Year: %d, DayMonth: %.2lf", pUser->get_name().c_str(), pUser->get_year(), pUser->get_day_month());
}

void User::add_user(User *pUsers[], const int &maxSize, int &currentSize, string name, int year, double dayMonth) {
	if (currentSize < maxSize) {
		*pUsers[currentSize] = User(name, year, dayMonth);
		(currentSize)++;
	}
}

/*User User::remove_user() {*/
/**/
/*}*/

void User::read_file(string filePath, User *pUsers[], const int &maxSize, int &currentSize) {
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
			const rapidjson::Value &user = usersArray[i];

			if (user.HasMember("name") && user["name"].IsString()
				&& user.HasMember("year") && user["year"].IsInt()
				&& user.HasMember("dayMonth") && user["dayMonth"].IsDouble()) {
				const string name = user["name"].GetString();
				const int year = user["year"].GetInt();
				const double dayMonth = user["dayMonth"].GetDouble();
				add_user(pUsers, maxSize, currentSize, name, year, dayMonth);
			}
		}
	}
}
