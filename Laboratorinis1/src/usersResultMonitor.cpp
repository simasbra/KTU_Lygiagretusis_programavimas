#include "usersResultMonitor.h"
#include "userResult.h"
#include "usersMonitor.h"
#include <cstdio>

using namespace std;

UsersResultMonitor::UsersResultMonitor(UsersMonitor *pUsersMonitor) : currentSize_(0), usersMonitor(pUsersMonitor) {}

UsersResultMonitor::~UsersResultMonitor() {}

int UsersResultMonitor::get_current_size() {
	return currentSize_;
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

void UsersResultMonitor::steal_generate_set_check_add_user_result() {
	User userTemporary = get_user_last_from_users_monitor();
	UserResult *pUserResultTemporary = new UserResult(userTemporary);
	pUserResultTemporary->set_hash(pUserResultTemporary->generate_sha256());
	if (!pUserResultTemporary->check_hash_ends_with_a_number()) {
		add_user_result_sorted(*pUserResultTemporary);
	}
	delete pUserResultTemporary;
}

void UsersResultMonitor::print_users_result() {
	for (int i = 0; i < currentSize_; i++) {
		UserResult::print_user_result(usersResult_[i]);
	}
}

void UsersResultMonitor::print_users_result_to_file(const string &filePath) {
	FILE *pFile = fopen(filePath.c_str(), "w");
	if (pFile == NULL) {
		return;
	}

	fprintf(pFile, "%s\n", "+----------------------+------------+------------+------------------------------------------------------------------+");
	fprintf(pFile, "| %-20s | %-10s | %-10s | %-64s |\n", "Name", "Year", "Day Month", "SHA256 hash");
	fprintf(pFile, "%s\n", "+----------------------+------------+------------+------------------------------------------------------------------+");
	for (int i = 0; i < currentSize_; i++) {
		User user = usersResult_[i].get_user();
		fprintf(pFile, "| %-20s | %10d | %10.2lf | %-64s |\n",
			user.get_name().c_str(), user.get_year(), user.get_day_month(), usersResult_[i].get_hash().c_str());
	}
	fprintf(pFile, "%s\n", "+----------------------+------------+------------+------------------------------------------------------------------+");
	fprintf(pFile, "| Users total: %-100d |\n", currentSize_);
	fprintf(pFile, "%s\n", "+-------------------------------------------------------------------------------------------------------------------+");
	
}
