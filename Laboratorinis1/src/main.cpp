#include "usersMonitor.h"
#include "usersResultMonitor.h"
#include <cstdio>
#include <sys/wait.h>

using namespace std;

const string filePathData = "data/IFF22_BradaitisV_L1_dat_1.json";
const string filePathResult = "results/IFF22_BradaitisV_L1_rez.txt";

int main(void) {
	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(pUsersMonitor);

	const int USER_COUNT = pUsersMonitor->get_file_user_count(filePathData);
	/*const int MAX_THREAD_COUNT = USER_COUNT_1 / 4;*/
	int iteration = 0;
	/*int currentThreadCount = 0;*/

	while (pUsersResultMonitor->get_users_processed() < USER_COUNT) {
		if (pUsersMonitor->get_users_read() < USER_COUNT) {
			pUsersMonitor->read_file(filePathData);
		}
		pUsersResultMonitor->steal_generate_set_check_add_user_result();
		printf("Iteration: %4d Read: %4d Processed: %4d\n", iteration++, pUsersMonitor->get_users_read(), pUsersResultMonitor->get_users_processed());
	}

	/*printf("\nUser Monitor: \n");*/
	/*pUsersMonitor->print_users();*/
	/*printf("User Monitor count: %d\n", pUsersMonitor->get_current_size());*/
	/*printf("\nUser Result Monitor: \n");*/
	/*pUsersResultMonitor->print_users_result();*/
	/*printf("User Result Monitor count: %d\n", pUsersResultMonitor->get_current_size());*/

	pUsersResultMonitor->print_users_result_to_file(filePathResult);

	delete pUsersMonitor;
	delete pUsersResultMonitor;
	return 0;
}
