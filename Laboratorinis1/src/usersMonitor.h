#ifndef USERS_MONITOR
#define USERS_MONITOR

#include "user.h"

class UsersMonitor {
private:
	static const int maxSize = 100;
	int currentSize;
	User users[maxSize];

public:
	UsersMonitor();
	~UsersMonitor();

	int get_current_size();
	void print_users();
	void add_user_last(User userNew);
	void add_user_sorted(User userNew);
	User remove_user_last();

	void read_file(std::string filePath);
};

#endif
