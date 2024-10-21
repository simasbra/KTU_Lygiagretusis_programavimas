#ifndef USER_DATA_MONITOR
#define USER_DATA_MONITOR

#include <pthread.h>
#include "user.h"

typedef struct UserDataMonitor {
	User		users[16];
	unsigned int	currentSize;
	unsigned int	usersAdded;

	pthread_mutex_t	mutex;
	pthread_cond_t	conditionalUserAdded;
	pthread_cond_t	conditionalUserRemoved;
} UserDataMonitor;

void		UDM_initialize_user_data_monitor	(UserDataMonitor *pUserDataMonitor);
unsigned int	UDM_get_monitor_max_size		(UserDataMonitor *pUserDataMonitor);
void		UDM_initialize_pthreads			(UserDataMonitor *pUserDataMonitor);
void		UDM_destroy_pthreads			(UserDataMonitor *pUserDataMonitor);

pthread_cond_t	*UDM_get_conditional_user_added		(UserDataMonitor *pUserDataMonitor);
pthread_cond_t	*UDM_get_conditional_user_removed	(UserDataMonitor *pUserDataMonitor);

void		UDM_add_user_last			(UserDataMonitor *pUserDataMonitor, User userNew);
void		UDM_add_user_sorted			(UserDataMonitor *pUserDataMonitor, User userNew);
User		UDM_remove_user_last			(UserDataMonitor *pUserDataMonitor);

int		UDM_check_is_space_available		(UserDataMonitor *pUserDataMonitor);
void		UDM_print_users				(UserDataMonitor *pUserDataMonitor);

#endif
