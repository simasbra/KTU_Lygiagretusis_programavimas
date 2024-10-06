#include "openmpMonitor.h"

OpenMPMonitor::OpenMPMonitor(int usersToBeAdded) : currentSize_(0), usersProcessed_(0), usersToBeAdded_(usersToBeAdded) {
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

OpenMPMonitor::~OpenMPMonitor() {
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

unsigned int OpenMPMonitor::get_current_size() {
	return currentSize_;
}

unsigned int OpenMPMonitor::get_users_processed() {
	return usersProcessed_;
}

unsigned int OpenMPMonitor::get_users_to_be_added() {
	return usersToBeAdded_;
}

void OpenMPMonitor::increase_users_processed() {
	pthread_mutex_lock(&mutex_);
	usersProcessed_++;
	pthread_mutex_unlock(&mutex_);
}

void OpenMPMonitor::add_user_result_last(UserResult userResultNew) {
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

void OpenMPMonitor::add_user_result_sorted(UserResult userResultNew) {
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

UserResult OpenMPMonitor::remove_user_result_last() {
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

UserResult OpenMPMonitor::get_user_result_last() {
	pthread_mutex_lock(&mutex_);
	if (currentSize_ == 0) {
		pthread_mutex_unlock(&mutex_);
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[currentSize_ - 1];
	pthread_mutex_unlock(&mutex_);
	return userResultTemporary;
}

bool OpenMPMonitor::check_all_users_processed() {
	if (usersProcessed_ == usersToBeAdded_) {
		/*pthread_cond_signal(pUsersMonitor_->get_conditional_user_added());*/
		return true;
	}
	return false;
}

bool OpenMPMonitor::process_user_result(User *pUserNew) {
	if (!pUserNew->is_valid()) {
		return false;
	}

	UserResult *pUserResultTemporary = new UserResult(*pUserNew);
	string message = pUserResultTemporary->generate_string();
	string hashedOutput = pUserResultTemporary->hash_using_blake2b(pUserResultTemporary->hash_using_sha256(message));
	pUserResultTemporary->set_hash(hashedOutput);
	pUserResultTemporary->calculate_set_sum();
	increase_users_processed();
	if (!pUserResultTemporary->check_hash_ends_with_a_number()) {
		add_user_result_sorted(*pUserResultTemporary);
	}
	delete pUserResultTemporary;
	return true;
}

void OpenMPMonitor::print_users_result() {
	pthread_mutex_lock(&mutex_);
	for (unsigned int i = 0; i < currentSize_; i++) {
		printf("%-4d ", i + 1);
		usersResult_[i].print_user_result_with_sum();
	}
	pthread_mutex_unlock(&mutex_);
}

void OpenMPMonitor::print_users_result_to_file(const string &filePath) {
	pthread_mutex_lock(&mutex_);
	FILE *pFile = fopen(filePath.c_str(), "w");
	if (pFile == NULL) {
		pthread_mutex_unlock(&mutex_);
		return;
	}

	string dashes = "+----------------------+------------+------------+------------+----------------------------------------------------------------------------------------------------------------------------------+";
	fprintf(pFile, "%s\n", dashes.c_str());
	fprintf(pFile, "| %-20s | %-10s | %-10s | %-10s | %-128s |\n", "Name", "Year", "Day Month", "Sum", "SHA256 + BLAKE2b hash");
	fprintf(pFile, "%s\n", dashes.c_str());

	for (unsigned int i = 0; i < currentSize_; i++) {
		User user = usersResult_[i].get_user();
		fprintf(pFile, "| %-20s | %10d | %10.2lf | %10.2lf | %-128s |\n",
		  user.get_name().c_str(), user.get_year(), user.get_day_month(), usersResult_[i].get_sum(), usersResult_[i].get_hash().c_str());
	}

	fprintf(pFile, "%s\n", dashes.c_str());
	fprintf(pFile, "| Users total: %-164d |\n", currentSize_);
	fprintf(pFile, "%s\n", "+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+");
	fclose(pFile);
	pthread_mutex_unlock(&mutex_);
}
