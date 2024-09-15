#ifndef USERS_MONITOR
#define USERS_MONITOR

#include "user.h"

class UsersMonitor {
private:
	static const int MAX_SIZE_ = 16;
	User users_[MAX_SIZE_];
	int currentSize_;
	int usersAdded_;

public:
	UsersMonitor();
	~UsersMonitor();

	int get_max_size();
	int get_current_size();
	int get_users_added();
	void print_users();
	void add_user_last(User userNew);
	void add_user_sorted(User userNew);
	User remove_user_last();
};

#endif
