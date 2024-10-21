#ifndef USERS_RESULT_MONITOR
#define USERS_RESULT_MONITOR

#include "userDataMonitor.h"
#include "userResult.h"
#include <pthread.h>

typedef struct UserResultMonitor {
	unsigned int	currentSize;
	unsigned int	usersProcessed;
	unsigned int	usersToBeAdded;

	pthread_mutex_t	mutex;
	pthread_cond_t	conditionalUserResultAdded;

	UserDataMonitor *pUserDataMonitor;
	UserResult	usersResult[500];
} UserResultMonitor;

void		URM_initialize_user_result_monitor	(UserResultMonitor *pUserResultMonitor, UserDataMonitor *pUserDataMonitor, unsigned int usersToBeAdded);
void		URM_initialize_pthreads			(UserResultMonitor *pUserResultMonitor);
void		URM_destroy_pthreads			(UserResultMonitor *pUserResultMonitor);

unsigned int	URM_get_monitor_max_size		(UserResultMonitor *pUserResultMonitor);
void		URM_add_user_result_last		(UserResultMonitor *pUserResultMonitor, UserResult userResultNew);
void		URM_add_user_result_sorted		(UserResultMonitor *pUserResultMonitor, UserResult userResultNew);
UserResult	URM_remove_user_result_last		(UserResultMonitor *pUserResultMonitor);
int		URM_check_all_users_processed		(UserResultMonitor *pUserResultMonitor);
int		URM_process_user_result			(UserResultMonitor *pUserResultMonitor, User *pUserNew);

int		URM_check_data_monitor_all_users_added	(UserResultMonitor *pUserResultMonitor);
User		URM_get_user_last_from_users_monitor	(UserResultMonitor *pResultMonitor);

void		URM_print_users_result			(UserResultMonitor *pUserResultMonitor);
void		URM_print_users_result_to_file		(UserResultMonitor *pUserResultMonitor, const char *pFilePath[]);

#endif
