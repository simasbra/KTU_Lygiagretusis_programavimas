#include "userResultMonitor.h"
#include "userResult.h"
#include <stdlib.h>
#include <string.h>

unsigned int URM_get_monitor_max_size(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return 0;
	return sizeof(pResultMonitor->usersResult) / sizeof(pResultMonitor->usersResult[0]);
}

void URM_initialize_pthreads(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor)
          exit(1);
        int error;
	if ((error = pthread_mutex_init(&pResultMonitor->mutex, NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&pResultMonitor->conditionalUserResultAdded, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void URM_destroy_pthreads(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_destroy(&pResultMonitor->mutex)) != 0) {
		printf("Mutex cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&pResultMonitor->conditionalUserResultAdded)) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void URM_add_user_result_last(UserResultMonitor *pResultMonitor, UserResult userResultNew) {
	if (!pResultMonitor) return;
}

void URM_add_user_result_sorted(UserResultMonitor *pResultMonitor, UserResult userResultNew) {
	if (!pResultMonitor) return;
}

UserResult URM_remove_user_result_last(UserResultMonitor *pResultMonitor);
int URM_check_all_users_processed(UserResultMonitor *pResultMonitor);
int URM_process_user_result(UserResultMonitor *pResultMonitor, User *pUserNew);

User URM_get_user_last_from_data_monitor(UserResultMonitor *pResultMonitor);
unsigned int URM_get_data_monitor_current_size(UserResultMonitor *pResultMonitor);
int URM_check_data_monitor_all_users_added(UserResultMonitor *pResultMonitor);

void URM_print_users_result(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return;
}

void URM_print_users_result_to_file(UserResultMonitor *pResultMonitor, const char *pFilePath[]) {
	if (!pResultMonitor) return;
}
