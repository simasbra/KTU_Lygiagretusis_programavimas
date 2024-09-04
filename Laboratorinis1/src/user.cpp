#include "user.h"

using namespace std;

User::User() : name(""), year(0), dayMonth(0) {};

User::User(string name, int year, double dayMonth) : name(name), year(year), dayMonth(dayMonth) {};

string User::getName() {
	return name;
}

int User::getYear() {
	return year;
}

double User::getDayMonth() {
	return dayMonth;
}

void User::setName(string name) {
	this->name = name;
}

void User::setYear(int year) {
	this->year = year;
}

void User::setDayMonth(double dayMonth) {
	this->dayMonth = dayMonth;
}

void User::addUser(User *users[], int *maxSize, int *currentSize, string name, int year, double dayMonth) {
	if (*currentSize <= *maxSize) {
		*users[*currentSize] = User(name, year, dayMonth);
		currentSize++;
	}
}
