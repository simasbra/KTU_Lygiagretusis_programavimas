#ifndef USERS_MONITOR
#define USERS_MONITOR

#include "user.h"

class UsersMonitor {
private:
	static const int MAX_SIZE_ = 10;
	User users_[MAX_SIZE_];
	int currentSize_;
	int usersRead_;

public:
	UsersMonitor();
	~UsersMonitor();

	int get_current_size();
	int get_users_read();
	void print_users();
	void add_user_last(User userNew);
	void add_user_sorted(User userNew);
	User remove_user_last();

	int get_file_user_count(const std::string &filePath);
	void read_file(const std::string &filePath);
};

#endif
