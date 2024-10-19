#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userResult.h"
#include <pthread.h>

typedef struct UserResultMonitor {
	unsigned int currentSize;
	unsigned int usersProcessed;
	unsigned int usersToBeAdded;

	pthread_mutex_t mutex_;
	pthread_cond_t conditionalUserAdded_;

	UserResult usersResult_[500];
} UserResultMonitor;

#endif
