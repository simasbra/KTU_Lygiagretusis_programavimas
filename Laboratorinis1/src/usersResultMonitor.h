#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userResult.h"
#include "usersMonitor.h"
#include <string>

class UsersResultMonitor {
private:
	static const int MAX_SIZE_ = 500;
	UserResult usersResult_[MAX_SIZE_];
	int currentSize_;
	int usersProcessed_;

	UsersMonitor *usersMonitor;

public:
	UsersResultMonitor(UsersMonitor *usersMonitor);
	~UsersResultMonitor();

	int get_current_size();
	int get_users_processed();
	void add_user_result_last(UserResult userResultNew);
	void add_user_result_sorted(UserResult userResultNew);
	UserResult remove_user_result_last();
	UserResult get_user_result_last();
	User get_user_last_from_users_monitor();

	void steal_generate_set_check_add_user_result();
	void print_users_result();
	void print_users_result_to_file(const std::string &filePath);
};

#endif
