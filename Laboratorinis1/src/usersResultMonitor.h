#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userResult.h"
#include "usersMonitor.h"

class UsersResultMonitor {
private:
	static const int MAX_SIZE_ = 1000;
	int currentSize_;
	UserResult usersResult_[MAX_SIZE_];

	UsersMonitor *usersMonitor;

public:
	UsersResultMonitor(UsersMonitor *usersMonitor);
	~UsersResultMonitor();

	void print_users();
	void add_user_result_last(UserResult userResultNew);
	void add_user_result_sorted(UserResult userResultNew);
	UserResult remove_user_result_last();
	UserResult get_user_result_last();

	User get_user_last_from_users_monitor();
};

#endif
