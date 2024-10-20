#include "userResultMonitor.h"
#include "user.h"
#include "userDataMonitor.h"
#include "userResult.h"
#include <stdlib.h>
#include <string.h>

unsigned int URM_get_monitor_max_size(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return 0;
	return sizeof(pResultMonitor->usersResult) / sizeof(pResultMonitor->usersResult[0]);
}

void URM_initialize_pthreads(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) exit(EXIT_FAILURE);
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

UserResult URM_remove_user_result_last(UserResultMonitor *pResultMonitor) {
	UserResult userResultEmpty;
	if (!pResultMonitor) return userResultEmpty;

	pthread_mutex_lock(&pResultMonitor->mutex);
	if (pResultMonitor->currentSize == 0) {
		pthread_mutex_unlock(&pResultMonitor->mutex);
		return userResultEmpty;
	}

	UserResult userResultTemporary = pResultMonitor->usersResult[--pResultMonitor->currentSize];
	pResultMonitor->usersResult[pResultMonitor->currentSize] = userResultEmpty;
	pthread_mutex_unlock(&pResultMonitor->mutex);
	return userResultTemporary;
}

int URM_check_all_users_processed(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return 0;
	int processed = 0;
	pthread_mutex_lock(&pResultMonitor->mutex);
	processed = pResultMonitor->usersProcessed == pResultMonitor->usersToBeAdded;
	pthread_mutex_unlock(&pResultMonitor->mutex);
	return processed;
}

int URM_process_user_result(UserResultMonitor *pResultMonitor, User *pUserNew) {
	if (!pResultMonitor || !pUserNew || !U_is_valid(pUserNew)) return 0;

	int wasUserAdded = 0;
	UserResult userResult;
	char message[16];
	UR_generate_string(&userResult, &message);
	char hashedOutput[128];
	strcpy(userResult.hash, hashedOutput);
	// increase processed
	if (UR_check_hash_ends_with_a_number(&userResult)) {
		URM_add_user_result_sorted(pResultMonitor, userResult);
		wasUserAdded = 1;
	}
	return wasUserAdded;
}

int URM_check_data_monitor_all_users_added(UserResultMonitor *pResultMonitor) {
	if (pResultMonitor->pUserDataMonitor->usersAdded == pResultMonitor->usersToBeAdded) {
		pthread_cond_broadcast(&pResultMonitor->pUserDataMonitor->conditionalUserAdded);
		return 1;
	}
	return 0;
}

void URM_print_users_result(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return;
	pthread_mutex_lock(&pResultMonitor->mutex);

	char dashes[] = "+----------------------+------------+------------+----------------------------------------------------------------------------------------------------------------------------------+";
	printf("%s\n", dashes);
	printf("| %-20s | %-10s | %-10s | %-128s |\n", "Name", "Year", "Day Month", "SHA256 + BLAKE2b hash");
	printf("%s\n", dashes);

	for (unsigned int i = 0; i < pResultMonitor->currentSize; i++) {
		UR_print_user_result(&pResultMonitor->usersResult[i]);
	}

	printf("%s\n", dashes);
	printf("| Users total: %-164d |\n", pResultMonitor->currentSize);
	printf("%s\n", "+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+");
	pthread_mutex_unlock(&pResultMonitor->mutex);
}

void URM_print_users_result_to_file(UserResultMonitor *pResultMonitor, const char *pFilePath[]) {
	if (!pResultMonitor || !pFilePath) return;
	pthread_mutex_lock(&pResultMonitor->mutex);

	FILE *pFile = fopen(*pFilePath, "w");
	if (pFile == NULL) {
	pthread_mutex_lock(&pResultMonitor->mutex);
		return;
	}

	char dashes[] = "+----------------------+------------+------------+----------------------------------------------------------------------------------------------------------------------------------+";
	fprintf(pFile, "%s\n", dashes);
	fprintf(pFile, "| %-20s | %-10s | %-10s | %-128s |\n", "Name", "Year", "Day Month", "SHA256 + BLAKE2b hash");
	fprintf(pFile, "%s\n", dashes);

	for (unsigned int i = 0; i < pResultMonitor->currentSize; i++) {
		UR_print_user_result_to_file(&pResultMonitor->usersResult[i], pFile);
	}

	fprintf(pFile, "%s\n", dashes);
	fprintf(pFile, "| Users total: %-164d |\n", pResultMonitor->currentSize);
	fprintf(pFile, "%s\n", "+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+");
	fclose(pFile);
	pthread_mutex_unlock(&pResultMonitor->mutex);
}
