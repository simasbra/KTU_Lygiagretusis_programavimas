#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userResult.h"
#include "usersMonitor.h"

class UsersResultMonitor {
private:
	static const int maxSize = 1000;
	int currentSize;
	UserResult users[maxSize];

	UsersMonitor *usersMonitor;

public:
	UsersResultMonitor(UsersMonitor *usersMonitor);
	~UsersResultMonitor();

	void print_users();
	void add_user_last(UserResult userResultNew);
	void add_user_sorted(UserResult userResultNew);
	UserResult remove_user_last();

	User get_user_last_from_users_monitor();
};

#endif
