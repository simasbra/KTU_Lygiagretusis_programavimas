#include "userDataMonitor.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UDM_initialize_user_data_monitor(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return;
	pDataMonitor->currentSize = 0;
	pDataMonitor->usersAdded = 0;
	UDM_initialize_pthreads(pDataMonitor);
}

unsigned int UDM_get_monitor_max_size(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return 0;
	return sizeof(pDataMonitor->users) / sizeof(pDataMonitor->users[0]);
}

void UDM_initialize_pthreads(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_init(&pDataMonitor->mutex, NULL)) != 0) {
		printf("Mutex cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&pDataMonitor->conditionalUserAdded, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_init(&pDataMonitor->conditionalUserRemoved, NULL)) != 0) {
		printf("Conditional variable cannot be created: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

void UDM_destroy_pthreads(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) exit(EXIT_FAILURE);
	int error;
	if ((error = pthread_mutex_destroy(&pDataMonitor->mutex)) != 0) {
		printf("Mutex cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&pDataMonitor->conditionalUserAdded)) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
	if ((error = pthread_cond_destroy(&pDataMonitor->conditionalUserRemoved)) != 0) {
		printf("Conditional variable cannot be destroyed: [%s]", strerror(error));
		exit(EXIT_FAILURE);
	}
}

pthread_mutex_t *UDM_get_mutex(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return NULL;
	return &pDataMonitor->mutex;
}

pthread_cond_t *UDM_get_conditional_user_added(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return NULL;
	return &pDataMonitor->conditionalUserAdded;
}

pthread_cond_t *UDM_get_conditional_user_removed(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return NULL;
	return &pDataMonitor->conditionalUserRemoved;
}

void UDM_add_user_last(UserDataMonitor *pDataMonitor, User userNew) {
	if (!pDataMonitor) return;
	pthread_mutex_lock(&pDataMonitor->mutex);
	while (!UDM_check_is_space_available(pDataMonitor)) {
		pthread_cond_wait(&pDataMonitor->conditionalUserRemoved, &pDataMonitor->mutex);
	}

	pDataMonitor->users[pDataMonitor->currentSize] = userNew;
	pDataMonitor->currentSize++;
	pDataMonitor->usersAdded++;
	pthread_cond_signal(&pDataMonitor->conditionalUserAdded);
	pthread_mutex_unlock(&pDataMonitor->mutex);
}

void UDM_add_user_sorted(UserDataMonitor *pDataMonitor, User userNew) {
	if (!pDataMonitor) return;
	pthread_mutex_lock(&pDataMonitor->mutex);
	const unsigned int MAX_SIZE = UDM_get_monitor_max_size(pDataMonitor);
	while (pDataMonitor->currentSize == MAX_SIZE) {
		pthread_cond_wait(&pDataMonitor->conditionalUserRemoved, &pDataMonitor->mutex);
	}

	int i;
	for (i = pDataMonitor->currentSize - 1; i >= 0; i--) {
		if (pDataMonitor->users[i].year < userNew.year) {
			break;
		}
		pDataMonitor->users[i + 1] = pDataMonitor->users[i];
	}
	pDataMonitor->users[i + 1] = userNew;
	pDataMonitor->currentSize++;
	pDataMonitor->usersAdded++;
	pthread_cond_signal(&pDataMonitor->conditionalUserAdded);
	pthread_mutex_unlock(&pDataMonitor->mutex);
}

User UDM_remove_user_last(UserDataMonitor *pDataMonitor) {
	User userEmpty = {0};
	if (!pDataMonitor) return userEmpty;

	pthread_mutex_lock(&pDataMonitor->mutex);
	if (pDataMonitor->currentSize == 0) {
		pthread_mutex_unlock(&pDataMonitor->mutex);
		return userEmpty;
	}

	User userTemporary = pDataMonitor->users[--pDataMonitor->currentSize];
	pDataMonitor->users[pDataMonitor->currentSize] = userEmpty;
	pthread_cond_signal(&pDataMonitor->conditionalUserRemoved);
	pthread_mutex_unlock(&pDataMonitor->mutex);
	return userTemporary;
}

int UDM_check_is_space_available(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return 0;
	int spaceAvailable = 0;
	pthread_mutex_lock(&pDataMonitor->mutex);
	spaceAvailable = pDataMonitor->currentSize < UDM_get_monitor_max_size(pDataMonitor);
	pthread_mutex_unlock(&pDataMonitor->mutex);
	return spaceAvailable;
}

void UDM_print_users(UserDataMonitor *pDataMonitor) {
	if (!pDataMonitor) return;
	pthread_mutex_lock(&pDataMonitor->mutex);
	for (unsigned int i = 0; i < pDataMonitor->currentSize; i++) {
		printf("%4d ", i + 1);
		U_print_user(&pDataMonitor->users[i]);
	}
	pthread_mutex_unlock(&pDataMonitor->mutex);
}
