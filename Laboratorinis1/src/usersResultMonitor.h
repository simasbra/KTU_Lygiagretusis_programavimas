#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userResult.h"

class UsersResultMonitor {
public:
	static const int maxSize = 1000;
	int currentSize;
	UserResult users[maxSize];

	UsersResultMonitor();
	~UsersResultMonitor();

	void print_users();
	void add_user(UserResult userResultNew);
	void add_user_sorted(UserResult userResultNew);
	UserResult remove_user_last();
};

#endif
