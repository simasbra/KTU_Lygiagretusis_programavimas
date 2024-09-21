#include "usersResultMonitor.h"
#include <pthread.h>

UsersResultMonitor::UsersResultMonitor(int usersToBeAdded, UsersMonitor *pUsersMonitor) : currentSize_(0), usersProcessed_(0), usersToBeAdded_(usersToBeAdded), pUsersMonitor_(pUsersMonitor) {
	int error;
	if ((error = pthread_mutex_init(&mutex_, NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&conditionalUserAdded_, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

UsersResultMonitor::~UsersResultMonitor() {
	int error;
	if ((error = pthread_mutex_destroy(&mutex_)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&conditionalUserAdded_)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

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
	pthread_mutex_lock(&mutex_);
	usersProcessed_++;
	pthread_mutex_unlock(&mutex_);
}

void UsersResultMonitor::add_user_result_last(UserResult userResultNew) {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == MAX_SIZE_) {
		printf("UsersResultMonitor is full\n");
		pthread_mutex_unlock(&mutex_);
		return;
	}

	usersResult_[currentSize_] = userResultNew;
	currentSize_++;
	pthread_cond_signal(&conditionalUserAdded_);
	pthread_mutex_unlock(&mutex_);
}

void UsersResultMonitor::add_user_result_sorted(UserResult userResultNew) {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == MAX_SIZE_) {
		printf("UsersResultMonitor is full\n");
		pthread_mutex_unlock(&mutex_);
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
	pthread_cond_signal(&conditionalUserAdded_);
	pthread_mutex_unlock(&mutex_);
}

UserResult UsersResultMonitor::remove_user_result_last() {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == 0) {
		pthread_mutex_unlock(&mutex_);
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[--currentSize_];
	usersResult_[currentSize_] = UserResult();
	pthread_mutex_unlock(&mutex_);
	return userResultTemporary;
}

UserResult UsersResultMonitor::get_user_result_last() {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == 0) {
		pthread_mutex_unlock(&mutex_);
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[currentSize_ - 1];
	pthread_mutex_unlock(&mutex_);
	return userResultTemporary;
}

bool UsersResultMonitor::check_all_users_processed() {
	return usersProcessed_ == usersToBeAdded_;
}

User UsersResultMonitor::get_user_last_from_users_monitor() {
	pthread_mutex_lock(&mutex_);
	while (get_users_monitor_current_size() == 0 && !check_user_monitor_all_users_added()) {
		pthread_cond_wait(pUsersMonitor_->get_conditional_user_added(), &mutex_);
	}
	User userTemporary = pUsersMonitor_->remove_user_last();
	pthread_mutex_unlock(&mutex_);
	return userTemporary;
}

unsigned int UsersResultMonitor::get_users_monitor_current_size() {
	return pUsersMonitor_->get_current_size();
}

bool UsersResultMonitor::check_user_monitor_all_users_added() {
	if (pUsersMonitor_->get_users_added() == usersToBeAdded_) {
		pthread_cond_broadcast(pUsersMonitor_->get_conditional_user_added());
		return true;
	}
	return false;
}

void UsersResultMonitor::print_users_result() {
	pthread_mutex_lock(&mutex_);
	for (unsigned int i = 0; i < currentSize_; i++) {
		printf("%-4d ", i + 1);
		usersResult_[i].print_user_result();
	}
	pthread_mutex_unlock(&mutex_);
}

void UsersResultMonitor::print_users_result_to_file(const string &filePath) {
	pthread_mutex_lock(&mutex_);
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
	fclose(pFile);
	pthread_mutex_unlock(&mutex_);
}
