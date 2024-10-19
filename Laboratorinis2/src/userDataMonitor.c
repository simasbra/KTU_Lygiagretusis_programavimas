#include "userDataMonitor.h"

unsigned int UM_get_max_size(UserDataMonitor *pUserDataMonitor) {
	if (pUserDataMonitor == NULL) {
		return 0;
	}
	return sizeof(pUserDataMonitor->users) / sizeof(pUserDataMonitor->users[0]);
}

pthread_mutex_t *UM_get_mutex(UserDataMonitor *pDataMonitor);
pthread_cond_t *UM_get_conditional_user_added(UserDataMonitor *pDataMonitor);
pthread_cond_t *UM_get_conditional_user_removed(UserDataMonitor *pDataMonitor);

void UM_add_user_last(UserDataMonitor *pDataMonitor, User userNew);
void UM_add_user_sorted(UserDataMonitor *pDataMonitor, User userNew);
User UM_remove_user_last(UserDataMonitor *pDataMonitor);

int UM_check_is_space_available(UserDataMonitor *pDataMonitor);
void UM_print_users(UserDataMonitor *pDataMonitor);
