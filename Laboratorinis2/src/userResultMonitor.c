#include "userResultMonitor.h"
#include "user.h"
#include "userDataMonitor.h"
#include "userResult.h"
#include <stdlib.h>
#include <string.h>

void URM_initialize_user_result_monitor(UserResultMonitor *pResultMonitor, UserDataMonitor *pDataMonitor, unsigned int usersToBeAdded) {
	if (!pResultMonitor || !pDataMonitor) return;
	pResultMonitor->pUserDataMonitor = pDataMonitor;
	pResultMonitor->currentSize = 0;
	pResultMonitor->usersProcessed = 0;
	pResultMonitor->usersToBeAdded = usersToBeAdded;
	URM_initialize_pthreads(pResultMonitor);
}

void URM_initialize_pthreads(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) exit(EXIT_FAILURE);
        int error;
	if ((error = pthread_mutex_init(&(pResultMonitor->mutex), NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&(pResultMonitor->conditionalUserResultAdded), NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void URM_destroy_pthreads(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_destroy(&(pResultMonitor->mutex))) != 0) {
		printf("Mutex cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&(pResultMonitor->conditionalUserResultAdded))) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

unsigned int URM_get_monitor_max_size(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return 0;
	return sizeof(pResultMonitor->usersResult) / sizeof(UserResult);
}

void URM_add_user_result_sorted(UserResultMonitor *pResultMonitor, UserResult userResultNew) {
	if (!pResultMonitor) return;
	pthread_mutex_lock(&(pResultMonitor->mutex));
	if (pResultMonitor->currentSize == URM_get_monitor_max_size(pResultMonitor)) {
		printf("UsersResultMonitor is full\n");
		pthread_mutex_unlock(&(pResultMonitor->mutex));
		return;
	}

	int i;
	for (i = pResultMonitor->currentSize - 1; i >= 0; i--) {
		if (strcmp(pResultMonitor->usersResult[i].hash, userResultNew.hash) <= 0) {
			break;
		}
		pResultMonitor->usersResult[i + 1] = pResultMonitor->usersResult[i];
	}
	pResultMonitor->usersResult[i + 1] = userResultNew;
	pResultMonitor->currentSize++;
	pthread_cond_signal(&(pResultMonitor->conditionalUserResultAdded));
	pthread_mutex_unlock(&(pResultMonitor->mutex));
}

UserResult URM_remove_user_result_last(UserResultMonitor *pResultMonitor) {
	UserResult userResultEmpty = {0};
	if (!pResultMonitor) return userResultEmpty;

	pthread_mutex_lock(&pResultMonitor->mutex);
	if (pResultMonitor->currentSize == 0) {
		pthread_mutex_unlock(&(pResultMonitor->mutex));
		return userResultEmpty;
	}

	UserResult userResultTemporary = pResultMonitor->usersResult[--pResultMonitor->currentSize];
	pResultMonitor->usersResult[pResultMonitor->currentSize] = userResultEmpty;
	pthread_mutex_unlock(&(pResultMonitor->mutex));
	return userResultTemporary;
}

int URM_check_all_users_processed(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return 0;
	return pResultMonitor->usersProcessed == pResultMonitor->usersToBeAdded;
}

int URM_process_user_result(UserResultMonitor *pResultMonitor, User *pUserNew) {
	if (!pResultMonitor || !pUserNew) return 0;

	int wasUserAdded = 0;
	UserResult userResult;
	userResult.user = *pUserNew;

	char message[16];
	UR_generate_string(&userResult, message);
	strcpy(userResult.hash, message);

	/*char hashedOutput[128];*/
	/*UR_hash_using_sha256(message, hashedOutput);*/
	/*strcpy(userResult.hash, hashedOutput);*/

	pResultMonitor->usersProcessed++;
	/*if (UR_check_hash_ends_with_a_number(&userResult)) {*/
		URM_add_user_result_sorted(pResultMonitor, userResult);
		wasUserAdded = 1;
	/*}*/
	return wasUserAdded;
}

User URM_get_user_last_from_users_monitor(UserResultMonitor *pResultMonitor) {
	pthread_mutex_lock(&(pResultMonitor->mutex));
	while (!URM_check_data_monitor_all_users_added(pResultMonitor) && !URM_check_all_users_processed(pResultMonitor) && pResultMonitor->pUserDataMonitor->currentSize == 0) {
		pthread_cond_wait(&(pResultMonitor->pUserDataMonitor->conditionalUserAdded), &(pResultMonitor->mutex));
	}
	User userTemporary = {0};
	if (pResultMonitor->pUserDataMonitor->currentSize > 0) {
		userTemporary = UDM_remove_user_last(pResultMonitor->pUserDataMonitor);
	}
	pthread_mutex_unlock(&(pResultMonitor->mutex));
	return userTemporary;
}

int URM_check_data_monitor_all_users_added(UserResultMonitor *pResultMonitor) {
	if (pResultMonitor->pUserDataMonitor->usersAdded == pResultMonitor->usersToBeAdded) {
		pthread_cond_broadcast(&(pResultMonitor->pUserDataMonitor->conditionalUserAdded));
		return 1;
	}
	return 0;
}

void URM_print_users_result(UserResultMonitor *pResultMonitor) {
	if (!pResultMonitor) return;
	pthread_mutex_lock(&(pResultMonitor->mutex));

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
	pthread_mutex_unlock(&(pResultMonitor->mutex));
}

void URM_print_users_result_to_file(UserResultMonitor *pResultMonitor, const char *pFilePath[]) {
	if (!pResultMonitor || !pFilePath) return;
	pthread_mutex_lock(&(pResultMonitor->mutex));

	FILE *pFile = fopen(*pFilePath, "w");
	if (pFile == NULL) {
		pthread_mutex_unlock(&(pResultMonitor->mutex));
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
	pthread_mutex_unlock(&(pResultMonitor->mutex));
}
