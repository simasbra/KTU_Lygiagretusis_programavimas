#include "usersMonitor.h"
#include <cstdlib>
#include <pthread.h>

UsersMonitor::UsersMonitor() : currentSize_(0), usersAdded_(0) {
	int error;
	if ((error = pthread_mutex_init(&mutex_, NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&conditionalUserAdded_, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&conditionalUserRemoved_, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

UsersMonitor::~UsersMonitor() {
	int error;
	if ((error = pthread_mutex_destroy(&mutex_)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&conditionalUserAdded_)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&conditionalUserRemoved_)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

int UsersMonitor::get_max_size() {
	return MAX_SIZE_;
}

unsigned int UsersMonitor::get_current_size() {
	return currentSize_;
}

unsigned int UsersMonitor::get_users_added() {
	return usersAdded_;
}

pthread_mutex_t * UsersMonitor::get_mutex() {
	return &mutex_;
}

pthread_cond_t * UsersMonitor::get_conditional_user_added() {
	return &conditionalUserAdded_;
}

pthread_cond_t * UsersMonitor::get_conditional_user_removed() {
	return &conditionalUserRemoved_;
}

void UsersMonitor::add_user_last(User userNew) {
	pthread_mutex_lock(&mutex_);
	while (!check_is_space_available()) {
		pthread_cond_wait(&conditionalUserRemoved_, &mutex_);
	}

	users_[currentSize_] = userNew;
	currentSize_++;
	usersAdded_++;
	pthread_cond_signal(&conditionalUserAdded_);
	pthread_mutex_unlock(&mutex_);
}

void UsersMonitor::add_user_sorted(User userNew) {
	pthread_mutex_lock(&mutex_);
	while (currentSize_ == MAX_SIZE_) {
		pthread_cond_wait(&conditionalUserRemoved_, &mutex_);
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
	pthread_cond_signal(&conditionalUserAdded_);
	pthread_mutex_unlock(&mutex_);
}

User UsersMonitor::remove_user_last() {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == 0) {
		pthread_mutex_unlock(&mutex_);
		return User();
	}

	User userTemporary = users_[--currentSize_];
	users_[currentSize_] = User();
	pthread_cond_signal(&conditionalUserRemoved_);
	pthread_mutex_unlock(&mutex_);
	return userTemporary;
}

bool UsersMonitor::check_is_space_available() {
	return currentSize_ < MAX_SIZE_;
}

void UsersMonitor::print_users() {
	pthread_mutex_lock(&mutex_);
	for (unsigned int i = 0; i < currentSize_; i++) {
		printf("%4d ", i + 1);
		users_[i].print_user();
	}
	pthread_mutex_unlock(&mutex_);
}
