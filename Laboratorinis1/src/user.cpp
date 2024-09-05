#include "user.h"

using namespace std;

User::User() : name(""), year(0), dayMonth(0) {}

User::User(string name, int year, double dayMonth) : name(name), year(year), dayMonth(dayMonth) {}

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
	if (currentSize <= maxSize) {
		*pUsers[currentSize] = User(name, year, dayMonth);
		(currentSize)++;
	}
}
