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
	name_ = name;
}

void User::set_year(int year) {
	year_ = year;
}

void User::set_day_month(double dayMonth) {
	dayMonth_ = dayMonth;
}

bool User::is_valid() {
	if (!name_.empty() && year_ > 0 && dayMonth_ > 0) {
		return true;
	}
	return false;
}

void User::print_user(User &user) {
	printf("Name: %-20s Year: %10d DayMonth: %10.2lf\n", user.get_name().c_str(), user.get_year(), user.get_day_month());
}
