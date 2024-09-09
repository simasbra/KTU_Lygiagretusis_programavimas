#include "user.h"

using namespace std;

int main(void) {
	const int maxSize = 10;
	int currentSize = 0;
	User *pUsers = new User[maxSize];

	/*User::add_user(&pUsers, maxSize, currentSize, "Petras", 2023, 30.03);*/

	for (int i = 0; i < currentSize; i++) {
		User::print_user(&pUsers[i]);
		/*User removedUser = pUsers[i].remove_user();*/
	}

	User::read_file("data/Simas_BradaitisV_L1_dat_1.json");

	delete[] pUsers;
	return 0;
}
