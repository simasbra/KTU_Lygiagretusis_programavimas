#include "usersResultMonitor.h"
#include "userResult.h"
#include "usersMonitor.h"

using namespace std;

UsersResultMonitor::UsersResultMonitor(UsersMonitor *pUsersMonitor) : currentSize_(0), usersMonitor(pUsersMonitor) {}

UsersResultMonitor::~UsersResultMonitor() {}

void UsersResultMonitor::print_users() {
	for (int i = 0; i < currentSize_; i++) {
		UserResult::print_user_result(usersResult_[i]);
	}
}

void UsersResultMonitor::add_user_result_last(UserResult userResultNew) {
	if (currentSize_ >= MAX_SIZE_) {
		return;
	}

	usersResult_[currentSize_] = userResultNew;
	currentSize_++;
}

void UsersResultMonitor::add_user_result_sorted(UserResult userResultNew) {
	if (currentSize_ == 0) {
		add_user_result_last(userResultNew);
	}
	if (currentSize_ >= MAX_SIZE_) {
		return;
	}

	int i;
	for (i = currentSize_ - 1; i >= 0; i--) {
		if (usersResult_[i].get_hash() < userResultNew.get_hash()) {
			break;
		}
		usersResult_[i + 1] = usersResult_[i];
	}
	usersResult_[i + 1] = userResultNew;
	currentSize_++;
}

UserResult UsersResultMonitor::remove_user_result_last() {
	if (currentSize_ <= 0) {
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[--currentSize_];
	usersResult_[currentSize_] = UserResult();
	return userResultTemporary;
}

UserResult UsersResultMonitor::get_user_result_last() {
	if (currentSize_ <= 0) {
		return UserResult();
	}

	return usersResult_[currentSize_ - 1];
}

User UsersResultMonitor::get_user_last_from_users_monitor() {
	if (usersMonitor->get_current_size() <= 0) {
		return User();
	}

	return usersMonitor->remove_user_last();
}
