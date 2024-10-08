#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <omp.h>
#include "userResult.h"

using namespace std;

class OpenMPMonitor {
private:
	static const int MAX_SIZE_ = 500;
	UserResult usersResult_[MAX_SIZE_];
	unsigned int currentSize_;
	unsigned int usersProcessed_;
	unsigned int usersToBeAdded_;
	int sumInt_;
	float sumFloat_;

	omp_lock_t lock_;

public:
	OpenMPMonitor(int usersToBeAdded);
	~OpenMPMonitor();

	int get_sum_int();
	double get_sum_float();
	void set_sum_int(int newValue);
	void set_sum_float(float newValue);

	unsigned int get_current_size();
	unsigned int get_users_processed();
	unsigned int get_users_to_be_added();
	void increase_users_processed();
	void add_user_result_last(UserResult userResultNew);
	void add_user_result_sorted(UserResult userResultNew);
	UserResult remove_user_result_last();
	UserResult get_user_result_last();
	bool check_all_users_processed();
	bool process_user_result(User *pUserNew);

	void print_users_result();
	void print_users_result_to_file(const string &filePath);
};

#endif
