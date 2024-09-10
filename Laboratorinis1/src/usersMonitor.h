#ifndef USERS_MONITOR
#define USERS_MONITOR

#include "user.h"

class UsersMonitor {
public:
	static const int maxSize = 100;
	int currentSize;
	User users[maxSize];

	UsersMonitor();
	~UsersMonitor();

	void print_users();
	void add_user(User user);
	void add_user_sorted(User user);
	User remove_user_last();

	void read_file(std::string filePath);
};

#endif
