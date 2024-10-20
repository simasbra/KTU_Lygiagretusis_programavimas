#include "include/customMath.h"
#include "userDataMonitor.h"
#include "userResultMonitor.h"
#include "include/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

const char *FILE_PATH_RESULT = "results/IFF22_BradaitisS_L2_rez.txt\0";

void *create_thread(void *arg);
void read_file(const char *FILE_PATH_DATA, cJSON *pCjsonOutput);

int main(int argc, char **argv) {
	// Validate arguments
	if (argc != 2) {
		printf("Usage: %s <filepath>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	const char *FILE_PATH_DATA = argv[1];

	// Read file
	cJSON *pCjson;
	read_file(FILE_PATH_DATA, pCjson);

	// Initialize constants
	const unsigned int USER_COUNT = cJSON_GetObjectItemCaseSensitive(pCjson, "usersCount")->valueint;
	const int MAX_THREAD_COUNT = MAX(1, (int)MIN((int)(USER_COUNT / 4), (int)sysconf(_SC_NPROCESSORS_ONLN)));
	pthread_t threads[MAX_THREAD_COUNT];

	// Initialize monitors
	UserDataMonitor *pDataMonitor = (UserDataMonitor *)malloc(sizeof(UserDataMonitor));
	UserResultMonitor *pResultMonitor = (UserResultMonitor *)malloc(sizeof(UserResultMonitor));
	UDM_initialize_user_data_monitor(pDataMonitor);
	URM_initialize_user_result_monitor(pResultMonitor, pDataMonitor, USER_COUNT);

	// Create threads
	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_create(&threads[i], NULL, create_thread, &pResultMonitor);
		if (error != 0) {
			printf("Thread cannot be created: [%s]", strerror(error));
		}
	}

	// Join threads
	for (int i = 0; i < MAX_THREAD_COUNT; i++) {
		int error = pthread_join(threads[i], NULL);
		if (error != 0) {
			printf("Thread cannot be joined: [%s]", strerror(error));
		}
	}

	// Printing results
	URM_print_users_result_to_file(pResultMonitor, &FILE_PATH_RESULT);

	// Clean up
	cJSON_Delete(pCjson);
	free(pDataMonitor);
	free(pResultMonitor);

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

void read_file(const char *FILE_PATH_DATA, cJSON *pCjsonOutput) {
	// Open file
	FILE *pFile = fopen(FILE_PATH_DATA, "r");
	if (!pFile) {
		printf("File %s cannot be opened.\n", FILE_PATH_DATA, stderr);
		exit(EXIT_FAILURE);
	}

	// Get file size
	fseek(pFile, 0, SEEK_END);
	unsigned long fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// Read file contents to the buffer
	char *pBuffer = (char *)malloc(fileSize + 1);
	fread(pBuffer, 1, fileSize, pFile);
	pBuffer[fileSize] = '\0'; // Null terminating the string
	fclose(pFile);

	// Parse JSON data
	pCjsonOutput = cJSON_Parse(pBuffer);
	if (!pCjsonOutput) {
		const char *pError = cJSON_GetErrorPtr();
		if (pError) {
			printf("Error parsing JSON data: %s\n", pError, stderr);
		}
		cJSON_Delete(pCjsonOutput);
	}

	free(pBuffer);
}
