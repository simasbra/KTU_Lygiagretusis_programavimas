#include "usersMonitor.h"
#include "usersResultMonitor.h"

using namespace std;

const string filePath1 = "data/Simas_BradaitisV_L1_dat_1.json";

int main(void) {
	UsersMonitor *pUsersMonitor = new UsersMonitor();
	UsersResultMonitor *pUsersResultMonitor = new UsersResultMonitor(pUsersMonitor);

	pUsersMonitor->read_file(filePath1);

	delete pUsersMonitor;
	delete pUsersResultMonitor;
	return 0;
}
