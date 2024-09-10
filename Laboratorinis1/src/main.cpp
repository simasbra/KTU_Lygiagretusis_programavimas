#include "usersMonitor.h"

using namespace std;

const string filePath1 = "data/Simas_BradaitisV_L1_dat_1.json";

int main(void) {
	UsersMonitor *pUsersMonitor = new UsersMonitor();
	pUsersMonitor->read_file(filePath1);
	pUsersMonitor->print_users();

	delete pUsersMonitor;
	return 0;
}
