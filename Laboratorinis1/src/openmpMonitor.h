#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include "userResult.h"
#include "usersMonitor.h"
#include "userResult.h"
#include "usersMonitor.h"

using namespace std;

class OpenMPMonitor {
private:
	static const int MAX_SIZE_ = 500;
	UserResult usersResult_[MAX_SIZE_];
	unsigned int currentSize_;
	unsigned int usersProcessed_;
	unsigned int usersToBeAdded_;

	UsersMonitor *pUsersMonitor_;

	pthread_mutex_t mutex_;
	pthread_cond_t conditionalUserAdded_;

public:
	OpenMPMonitor(int usersToBeAdded, UsersMonitor *pUsersMonitor);
	~OpenMPMonitor();

	unsigned int get_current_size();
	unsigned int get_users_processed();
	unsigned int get_users_to_be_added();
	UsersMonitor *get_user_monitor_pointer();
	void increase_users_processed();
	void add_user_result_last(UserResult userResultNew);
	void add_user_result_sorted(UserResult userResultNew);
	UserResult remove_user_result_last();
	UserResult get_user_result_last();
	bool check_all_users_processed();
	bool process_user_result(User *pUserNew);

	User get_user_last_from_users_monitor();
	unsigned int get_users_monitor_current_size();
	bool check_user_monitor_all_users_added();

	void print_users_result();
	void print_users_result_to_file(const string &filePath);
};

#endif
