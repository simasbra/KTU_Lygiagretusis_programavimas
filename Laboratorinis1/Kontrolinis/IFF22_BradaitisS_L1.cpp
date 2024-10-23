/*
 * reikejo padaryt kad vienas thread raso skaicius 11, 12, 13..., kitas 101,102... i masyva a,
 * jeigu masyvas istusteja, skaiciuoja is naujo nuo 11 ar 101.
 * Trys threadai skaito is masyvo ir raso savo pid ir skaiciu kuri paeme.
 * Baigia darba kai suraso ir nuskaito 20 
 */

#include <cstdio>
#include <pthread.h>

class Monitor {
public:
	Monitor();
	~Monitor();

	int count;

	void add_number(int n);
	int get_last_number();
	bool is_done_writing();
	bool is_done_reading();

private:
	int a[10];
	int totalAdded;
	pthread_mutex_t mutex;
	pthread_cond_t added;
	pthread_cond_t removed;

};

Monitor::Monitor() : count(0), totalAdded(0) {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&added, NULL);
	pthread_cond_init(&removed, NULL);

}

Monitor::~Monitor() {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&added);
	pthread_cond_destroy(&removed);
}

void Monitor::add_number(int n) {
	pthread_mutex_lock(&mutex);
	while (count == 10) {
		pthread_cond_wait(&removed, &mutex);
	}
	if (totalAdded >= 20) {
		pthread_mutex_unlock(&mutex);
		return;
	};
	a[count++] = n;
	totalAdded++;
	pthread_mutex_unlock(&mutex);
}

int Monitor::get_last_number() {
	pthread_mutex_lock(&mutex);
	while (count == 0) {
		pthread_cond_wait(&added, &mutex);
	}
	int n = a[--count];
	a[count] = 0;
	pthread_cond_signal(&removed);
	pthread_mutex_unlock(&mutex);
	return n;
}

bool Monitor::is_done_reading() {
	bool isDone = totalAdded == 20 && count == 0;
	pthread_cond_signal(&removed);
	return isDone;
}

bool Monitor::is_done_writing() {
	bool isDone = totalAdded >= 20;
	pthread_cond_signal(&added);
	return isDone;
}


void *thread_write(void *args) {
	Monitor *pMonitor = (Monitor*)args;
	int i = 11;
	while (!pMonitor->is_done_writing()) {
		if (pMonitor->count == 0) i = 11;
		pMonitor->add_number(i);
		i++;
	}
}

void *thread_write2(void *args) {
	Monitor *pMonitor = (Monitor*)args;
	int i = 101;
	while (!pMonitor->is_done_writing()) {
		if (pMonitor->count == 0) i = 101;
		pMonitor->add_number(i);
		i++;
	}
}

void *thread_read(void *args) {
	Monitor *pMonitor = (Monitor*)args;
	while (!pMonitor->is_done_reading()) {
		int n = pMonitor->get_last_number();
		printf("%d\n", n);
	}
}

int main() {
	Monitor *pMonitor = new Monitor;

	pthread_t thread[5];
	pthread_create(&thread[0], NULL, thread_write, pMonitor);
	pthread_create(&thread[1], NULL, thread_write2, pMonitor);

	for (int i = 2; i < 5; i++) {
		pthread_create(&thread[i], NULL, thread_read, pMonitor);
	}

	for (int i = 0; i < 5; i++){
		pthread_join(thread[i], NULL);
	}
}
