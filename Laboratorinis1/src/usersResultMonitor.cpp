#include "usersResultMonitor.h"
#include "userResult.h"
#include "usersMonitor.h"
#include <cstdio>

using namespace std;

UsersResultMonitor::UsersResultMonitor(int usersToBeAdded, UsersMonitor *pUsersMonitor) : currentSize_(0), usersToBeAdded_(usersToBeAdded), pUsersMonitor_(pUsersMonitor) {}

UsersResultMonitor::~UsersResultMonitor() {}

unsigned int UsersResultMonitor::get_current_size() {
	return currentSize_;
}

unsigned int UsersResultMonitor::get_users_processed() {
	return usersProcessed_;
}

unsigned int UsersResultMonitor::get_users_to_be_added() {
	return usersToBeAdded_;
}

UsersMonitor * UsersResultMonitor::get_user_monitor_pointer() {
	return pUsersMonitor_;
}

void UsersResultMonitor::increase_users_processed() {
	usersProcessed_++;
}

void UsersResultMonitor::add_user_result_last(UserResult userResultNew) {
	if (currentSize_ == MAX_SIZE_) {
		return;
	}

	usersResult_[currentSize_] = userResultNew;
	currentSize_++;
}

void UsersResultMonitor::add_user_result_sorted(UserResult userResultNew) {
	if (currentSize_ == MAX_SIZE_) {
		return;
	}
	if (currentSize_ == 0) {
		add_user_result_last(userResultNew);
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

bool UsersResultMonitor::check_all_users_added() {
	return pUsersMonitor_->get_users_added() == usersToBeAdded_;
}

bool UsersResultMonitor::check_all_users_processed() {
	return usersProcessed_ == usersToBeAdded_;
}

User UsersResultMonitor::get_user_last_from_users_monitor() {
	if (pUsersMonitor_->get_current_size() <= 0) {
		return User();
	}

	return pUsersMonitor_->remove_user_last();
}

unsigned int UsersResultMonitor::get_users_monitor_current_size() {
	return pUsersMonitor_->get_current_size();
}

void UsersResultMonitor::print_users_result() {
	for (unsigned int i = 0; i < currentSize_; i++) {
		printf("%-4d ", i + 1);
		UserResult::print_user_result(usersResult_[i]);
	}
}

void UsersResultMonitor::print_users_result_to_file(const string &filePath) {
	FILE *pFile = fopen(filePath.c_str(), "w");
	if (pFile == NULL) {
		return;
	}

	string dashes = "+----------------------+------------+------------+------------------------------------------------------------------+";
	fprintf(pFile, "%s\n", dashes.c_str());
	fprintf(pFile, "| %-20s | %-10s | %-10s | %-64s |\n", "Name", "Year", "Day Month", "SHA256 hash");
	fprintf(pFile, "%s\n", dashes.c_str());

	for (unsigned int i = 0; i < currentSize_; i++) {
		User user = usersResult_[i].get_user();
		fprintf(pFile, "| %-20s | %10d | %10.2lf | %-64s |\n",
			user.get_name().c_str(), user.get_year(), user.get_day_month(), usersResult_[i].get_hash().c_str());
	}

	fprintf(pFile, "%s\n", dashes.c_str());
	fprintf(pFile, "| Users total: %-100d |\n", currentSize_);
	fprintf(pFile, "%s\n", "+-------------------------------------------------------------------------------------------------------------------+");
	
}
