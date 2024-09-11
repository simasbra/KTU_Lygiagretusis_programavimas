#include <cstdio>
#include "user.h"

using namespace std;

User::User() : name_(""), year_(0), dayMonth_(0) {}

User::User(string name, int year, double dayMonth) : name_(name), year_(year), dayMonth_(dayMonth) {}

User::~User() {}

string User::get_name() {
	return name_;
}

int User::get_year() {
	return year_;
}

double User::get_day_month() {
	return dayMonth_;
}

void User::set_name(string name) {
	this->name_ = name;
}

void User::set_year(int year) {
	this->year_ = year;
}

void User::set_day_month(double dayMonth) {
	this->dayMonth_ = dayMonth;
}

void User::print_user(User &user) {
	printf("Name: %s, Year: %d, DayMonth: %.2lf\n", user.get_name().c_str(), user.get_year(), user.get_day_month());
}
