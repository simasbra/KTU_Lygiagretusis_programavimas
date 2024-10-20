#include "include/customMath.h"
#include "userDataMonitor.h"
#include "userResultMonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

const char *FILE_PATH_RESULT = "results/IFF22_BradaitisS_L2_rez.txt\0";

void *create_thread(void *arg);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <filepath>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *FILE_PATH_DATA = argv[1];

	FILE *pFile = fopen(FILE_PATH_DATA, "r");
	if (!pFile) {
		printf("File %s cannot be opened.\n", FILE_PATH_DATA, stderr);
		exit(EXIT_FAILURE);
	}

	fclose(pFile);

	const unsigned int USER_COUNT = 1;
	const int MAX_THREAD_COUNT = MAX(1, (int)MIN((int)(USER_COUNT / 4), (int)sysconf(_SC_NPROCESSORS_ONLN)));

	pthread_t threads[MAX_THREAD_COUNT];

	UserDataMonitor dataMonitor;
	UserResultMonitor resultMonitor;
	UDM_initialize_user_data_monitor(&dataMonitor);
	URM_initialize_user_result_monitor(&resultMonitor, &dataMonitor, USER_COUNT);

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_create(&threads[i], NULL, create_thread, &resultMonitor);
		if (error != 0) {
			printf("Thread cannot be created: [%s]", strerror(error));
		}
	}

	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_join(threads[i], NULL);
		if (error != 0) {
			printf("Thread cannot be joined: [%s]", strerror(error));
		}
	}

	URM_print_users_result_to_file(&resultMonitor, &FILE_PATH_RESULT);

	return 0;
}

void *create_thread(void *arg) {
	UserResultMonitor *pResultMonitor = (UserResultMonitor *) arg;
	if (!pResultMonitor) return NULL;

	while (URM_check_all_users_processed(pResultMonitor)) {
		User userTemporary = UDM_remove_user_last(pResultMonitor->pUserDataMonitor);
		URM_process_user_result(pResultMonitor, &userTemporary);
	}
	return NULL;
}
