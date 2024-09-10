#include <cstdio>
#include "userResult.h"

using namespace std;

UserResult::UserResult() : user(User()), hash("") {}

UserResult::UserResult(User user, string hash) : user(user), hash(hash) {}

UserResult::~UserResult() {}

User UserResult::get_user() {
	return user;
}

string UserResult::get_hash() {
	return hash;
}

void UserResult::set_user(User newUser) {
	user = newUser;
}

void UserResult::set_hash(string hash) {
	UserResult::hash = hash;
}

void UserResult::print_user_result(UserResult &userResult) {
	printf("Name: %s, Year: %d, DayMonth: %.2lf\n", userResult.user.get_name().c_str(), userResult.user.get_year(), userResult.user.get_day_month());
}
