#include "openmpMonitor.h"
#include <omp.h>

OpenMPMonitor::OpenMPMonitor(int usersToBeAdded) : currentSize_(0), usersProcessed_(0), usersToBeAdded_(usersToBeAdded), sumInt_(0), sumFloat_(0) {
	omp_init_lock(&lock_);
}

OpenMPMonitor::~OpenMPMonitor() {
	omp_destroy_lock(&lock_);
}


int OpenMPMonitor::get_sum_int() {
	return sumInt_;
}
double OpenMPMonitor::get_sum_float() {
	return sumFloat_;
}

void OpenMPMonitor::set_sum_int(int newValue) {
	sumInt_ = newValue;
}

void OpenMPMonitor::set_sum_float(float newValue) {
	sumFloat_ = newValue;
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
	omp_set_lock(&lock_);
	usersProcessed_++;
	omp_unset_lock(&lock_);
}

void OpenMPMonitor::add_user_result_last(UserResult userResultNew) {
	omp_set_lock(&lock_);
	if (currentSize_ == MAX_SIZE_) {
		printf("UsersResultMonitor is full\n");
		omp_unset_lock(&lock_);
		return;
	}

	usersResult_[currentSize_] = userResultNew;
	currentSize_++;
	omp_unset_lock(&lock_);
}

void OpenMPMonitor::add_user_result_sorted(UserResult userResultNew) {
	omp_set_lock(&lock_);
	if (currentSize_ == MAX_SIZE_) {
		printf("UsersResultMonitor is full\n");
		omp_unset_lock(&lock_);
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
	omp_unset_lock(&lock_);
}

UserResult OpenMPMonitor::remove_user_result_last() {
	omp_set_lock(&lock_);
	if (currentSize_ == 0) {
		omp_unset_lock(&lock_);
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[--currentSize_];
	usersResult_[currentSize_] = UserResult();
	omp_unset_lock(&lock_);
	return userResultTemporary;
}

UserResult OpenMPMonitor::get_user_result_last() {
	omp_set_lock(&lock_);
	if (currentSize_ == 0) {
		omp_unset_lock(&lock_);
		return UserResult();
	}

	UserResult userResultTemporary = usersResult_[currentSize_ - 1];
	omp_unset_lock(&lock_);
	return userResultTemporary;
}

bool OpenMPMonitor::check_all_users_processed() {
	if (usersProcessed_ == usersToBeAdded_) {
		return true;
	}
	return false;
}

bool OpenMPMonitor::process_user_result(User *pUserNew) {
	if (!pUserNew->is_valid()) {
		return false;
	}

	bool wasUserAdded = false;
	UserResult *pUserResultTemporary = new UserResult(*pUserNew);
	string message = pUserResultTemporary->generate_string();
	string hashedOutput = pUserResultTemporary->hash_using_blake2b(pUserResultTemporary->hash_using_sha256(message));
	pUserResultTemporary->set_hash(hashedOutput);
	increase_users_processed();
	if (!pUserResultTemporary->check_hash_ends_with_a_number()) {
		add_user_result_sorted(*pUserResultTemporary);
		wasUserAdded = true;
	}
	delete pUserResultTemporary;
	return wasUserAdded;
}


void OpenMPMonitor::print_users_result() {
	omp_set_lock(&lock_);
	string dashesWithPluses = "+----------------------+------------+------------+----------------------------------------------------------------------------------------------------------------------------------+";
	string dashes = "+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+";
	printf("%s\n", dashesWithPluses.c_str());
	printf("| %-20s | %-10s | %-10s | %-128s |\n", "Name", "Year", "Day Month", "SHA256 + BLAKE2b hash");
	printf("%s\n", dashesWithPluses.c_str());

	for (unsigned int i = 0; i < currentSize_; i++) {
		usersResult_[i].print_user_result();
	}

	printf("%s\n", dashesWithPluses.c_str());
	printf("| Users total: %-164d |\n", currentSize_);
	printf("%s\n", dashes.c_str());
	printf("| Total sum of integers: %-154d |\n", sumInt_);
	printf("| Total sum of floats: %-156.2f |\n", sumFloat_);
	printf("%s\n", dashes.c_str());
	omp_unset_lock(&lock_);
}

void OpenMPMonitor::print_users_result_to_file(const string &filePath) {
	omp_set_lock(&lock_);
	FILE *pFile = fopen(filePath.c_str(), "w");
	if (pFile == NULL) {
		omp_unset_lock(&lock_);
		return;
	}

	string dashesWithPluses = "+----------------------+------------+------------+----------------------------------------------------------------------------------------------------------------------------------+";
	string dashes = "+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+";
	fprintf(pFile, "%s\n", dashesWithPluses.c_str());
	fprintf(pFile, "| %-20s | %-10s | %-10s | %-128s |\n", "Name", "Year", "Day Month", "SHA256 + BLAKE2b hash");
	fprintf(pFile, "%s\n", dashesWithPluses.c_str());

	for (unsigned int i = 0; i < currentSize_; i++) {
		usersResult_[i].print_user_result_to_file(pFile);
	}

	fprintf(pFile, "%s\n", dashesWithPluses.c_str());
	fprintf(pFile, "| Users total: %-164d |\n", currentSize_);
	fprintf(pFile, "%s\n", dashes.c_str());
	fprintf(pFile, "| Total sum of integers: %-154d |\n", sumInt_);
	fprintf(pFile, "| Total sum of floats: %-156.2f |\n", sumFloat_);
	fprintf(pFile, "%s\n", dashes.c_str());
	fclose(pFile);
	omp_unset_lock(&lock_);
}
