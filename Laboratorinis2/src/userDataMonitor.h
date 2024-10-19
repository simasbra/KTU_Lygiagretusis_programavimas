#ifndef USER_DATA_MONITOR
#define USER_DATA_MONITOR

#include <pthread.h>
#include "user.h"

typedef struct UserDataMonitor {
	User users[16];
	unsigned int currentSize;
	unsigned int usersAdded;

	pthread_mutex_t mutex;
	pthread_cond_t conditionalUserAdded;
	pthread_cond_t conditionalUserRemoved;
} UserDataMonitor;

unsigned int	UM_get_max_size(UserDataMonitor *pDataMonitor);

pthread_mutex_t	*UM_get_mutex(UserDataMonitor *pDataMonitor);
pthread_cond_t	*UM_get_conditional_user_added(UserDataMonitor *pDataMonitor);
pthread_cond_t	*UM_get_conditional_user_removed(UserDataMonitor *pDataMonitor);

void		UM_add_user_last(UserDataMonitor *pDataMonitor, User userNew);
void		UM_add_user_sorted(UserDataMonitor *pDataMonitor, User userNew);
User		UM_remove_user_last(UserDataMonitor *pDataMonitor);

int		UM_check_is_space_available(UserDataMonitor *pDataMonitor);
void		UM_print_users(UserDataMonitor *pDataMonitor);

#endif
