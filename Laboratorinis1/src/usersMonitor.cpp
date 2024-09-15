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

int UsersMonitor::get_max_size() {
	return MAX_SIZE_;
}

int UsersMonitor::get_current_size() {
	return currentSize_;
}

int UsersMonitor::get_users_added() {
	return usersAdded_;
}

void UsersMonitor::add_user_last(User userNew) {
	if (currentSize_ >= MAX_SIZE_) {
		return;
	}

	users_[currentSize_] = userNew;
	currentSize_++;
	usersAdded_++;
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
	usersAdded_++;
}

User UsersMonitor::remove_user_last() {
	if (currentSize_ <= 0) {
		return User();
	}

	User userTemporary = users_[--currentSize_];
	users_[currentSize_] = User();
	return userTemporary;
}
