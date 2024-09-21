#ifndef USERS_MONITOR
#define USERS_MONITOR

#include <pthread.h>
#include "user.h"

using namespace std;

class UsersMonitor {
private:
	static const int MAX_SIZE_ = 16;
	User users_[MAX_SIZE_];
	unsigned int currentSize_;
	unsigned int usersAdded_;

	pthread_mutex_t mutex_;
	pthread_cond_t conditionalUserAdded_;
	pthread_cond_t conditionalUserRemoved_;
public:

	UsersMonitor();
	~UsersMonitor();

	int get_max_size();
	unsigned int get_current_size();
	unsigned int get_users_added();
	pthread_mutex_t *get_mutex();
	pthread_cond_t *get_conditional_user_added();
	pthread_cond_t *get_conditional_user_removed();

	void add_user_last(User userNew);
	void add_user_sorted(User userNew);
	User remove_user_last();

	bool check_is_space_available();
	void print_users();
};

#endif
