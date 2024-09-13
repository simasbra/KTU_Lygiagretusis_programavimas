#include "usersMonitor.h"
#include "usersResultMonitor.h"
#include <sys/wait.h>

using namespace std;

const string filePathData1 = "data/IFF22_BradaitisV_L1_dat_1.json";
const string filePathResult1 = "results/IFF22_BradaitisV_L1_rez.txt";

int main(void) {
	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(pUsersMonitor);

	pUsersMonitor->read_file(filePathData1);
	printf("\nUser Result Monitor: \n");
	pUsersMonitor->print_users();
	printf("\n%d\n", pUsersMonitor->get_current_size());

	while (pUsersMonitor->get_current_size() >0) {
		pUsersResultMonitor->steal_generate_set_check_add_user_result();
	}

	printf("\nUser Result Monitor: \n");
	pUsersResultMonitor->print_users_result();
	printf("\n%d\n", pUsersResultMonitor->get_current_size());

	pUsersResultMonitor->print_users_result_to_file(filePathResult1);

	delete pUsersMonitor;
	delete pUsersResultMonitor;
	return 0;
}
