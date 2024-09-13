#ifndef USERS_MONITOR
#define USERS_MONITOR

#include "user.h"

class UsersMonitor {
private:
	static const int MAX_SIZE_ = 100;
	int currentSize_;
	User users_[MAX_SIZE_];

public:
	UsersMonitor();
	~UsersMonitor();

	int get_current_size();
	void print_users();
	void add_user_last(User userNew);
	void add_user_sorted(User userNew);
	User remove_user_last();

	void read_file(const std::string &filePath);
};

#endif
