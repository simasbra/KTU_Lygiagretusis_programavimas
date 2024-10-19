#include "userDataMonitor.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int UDM_get_max_size(UserDataMonitor *pUserDataMonitor) {
	if (!pUserDataMonitor) return 0;
	return sizeof(pUserDataMonitor->users) / sizeof(pUserDataMonitor->users[0]);
}

void UDM_initialize_pthreads(UserDataMonitor *pUserDataMonitor) {
	if (!pUserDataMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_init(&pUserDataMonitor->mutex, NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&pUserDataMonitor->conditionalUserAdded, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&pUserDataMonitor->conditionalUserRemoved, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void UDM_destroy_pthreads(UserDataMonitor *pUserDataMonitor) {
	if (!pUserDataMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_destroy(&pUserDataMonitor->mutex)) != 0) {
		printf("Mutex cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&pUserDataMonitor->conditionalUserAdded)) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&pUserDataMonitor->conditionalUserRemoved)) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void UDM_add_user_last(UserDataMonitor *pUserDataMonitor, User userNew) {
	if (!pUserDataMonitor) return;
	pthread_mutex_lock(&pUserDataMonitor->mutex);
	while (!UDM_check_is_space_available(pUserDataMonitor)) {
		pthread_cond_wait(&pUserDataMonitor->conditionalUserRemoved, &pUserDataMonitor->mutex);
	}

	pUserDataMonitor->users[pUserDataMonitor->currentSize] = userNew;
	pUserDataMonitor->currentSize++;
	pUserDataMonitor->usersAdded++;
	pthread_cond_signal(&pUserDataMonitor->conditionalUserAdded);
	pthread_mutex_unlock(&pUserDataMonitor->mutex);
}

void UDM_add_user_sorted(UserDataMonitor *pUserDataMonitor, User userNew) {
	if (!pUserDataMonitor) return;
	pthread_mutex_lock(&pUserDataMonitor->mutex);
	const unsigned int MAX_SIZE = UDM_get_max_size(pUserDataMonitor);
	while (pUserDataMonitor->currentSize == MAX_SIZE) {
		pthread_cond_wait(&pUserDataMonitor->conditionalUserRemoved, &pUserDataMonitor->mutex);
	}

	int i;
	for (i = pUserDataMonitor->currentSize - 1; i >= 0; i--) {
		if (pUserDataMonitor->users[i].year < userNew.year) {
			break;
		}
		pUserDataMonitor->users[i + 1] = pUserDataMonitor->users[i];
	}
	pUserDataMonitor->users[i + 1] = userNew;
	pUserDataMonitor->currentSize++;
	pUserDataMonitor->usersAdded++;
	pthread_cond_signal(&pUserDataMonitor->conditionalUserAdded);
	pthread_mutex_unlock(&pUserDataMonitor->mutex);
}

User UDM_remove_user_last(UserDataMonitor *pUserDataMonitor) {
	User userEmpty;
	if (!pUserDataMonitor) return userEmpty;

	pthread_mutex_lock(&pUserDataMonitor->mutex);
	if (pUserDataMonitor->currentSize == 0) {
		pthread_mutex_unlock(&pUserDataMonitor->mutex);
		return userEmpty;
	}

	User userTemporary = pUserDataMonitor->users[--pUserDataMonitor->currentSize];
	pUserDataMonitor->users[pUserDataMonitor->currentSize] = userEmpty;
	pthread_cond_signal(&pUserDataMonitor->conditionalUserRemoved);
	pthread_mutex_unlock(&pUserDataMonitor->mutex);
	return userTemporary;
}

int UDM_check_is_space_available(UserDataMonitor *pUserDataMonitor) {
	if (!pUserDataMonitor) return 0;
	return pUserDataMonitor->currentSize < UDM_get_max_size(pUserDataMonitor);
}

void UDM_print_users(UserDataMonitor *pUserDataMonitor) {
	if (!pUserDataMonitor) return;
	pthread_mutex_lock(&pUserDataMonitor->mutex);
	for (unsigned int i = 0; i < pUserDataMonitor->currentSize; i++) {
		printf("%4d ", i + 1);
		U_print_user(&pUserDataMonitor->users[i]);
	}
	pthread_mutex_unlock(&pUserDataMonitor->mutex);
}
