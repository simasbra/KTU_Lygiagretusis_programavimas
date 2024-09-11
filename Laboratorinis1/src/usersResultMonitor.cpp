#include "usersResultMonitor.h"
#include "userResult.h"
#include "usersMonitor.h"

using namespace std;

UsersResultMonitor::UsersResultMonitor(UsersMonitor *usersMonitor) : currentSize(0), usersMonitor(usersMonitor) {}

UsersResultMonitor::~UsersResultMonitor() {}

void UsersResultMonitor::print_users() {
	for (int i = 0; i < currentSize; i++) {
		UserResult::print_user_result(users[i]);
	}
}

void UsersResultMonitor::add_user_last(UserResult userResultNew) {
	if (currentSize >= maxSize) {
		return;
	}

	users[currentSize] = userResultNew;
	currentSize++;
}

void UsersResultMonitor::add_user_sorted(UserResult userResultNew) {
	if (currentSize == 0) {
		add_user_last(userResultNew);
	}
	if (currentSize >= maxSize) {
		return;
	}

	int i;
	for (i = currentSize - 1; i >= 0; i--) {
		if (users[i].get_hash() < userResultNew.get_hash()) {
			break;
		}
		users[i + 1] = users[i];
	}
	users[i + 1] = userResultNew;
	currentSize++;
}

UserResult UsersResultMonitor::remove_user_last() {
	if (currentSize <= 0) {
		return UserResult();
	}

	UserResult userResultTemporary = users[--currentSize];
	users[currentSize] = UserResult();
	return userResultTemporary;
}

User UsersResultMonitor::get_user_last_from_users_monitor() {
	if (usersMonitor->get_current_size() <= 0) {
		return User();
	}

	return usersMonitor->remove_user_last();
}
