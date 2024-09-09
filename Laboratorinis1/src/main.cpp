#include "user.h"

using namespace std;

const string filePath1 = "data/Simas_BradaitisV_L1_dat_1.json";

int main(void) {
	const int maxSize = 1000;
	int currentSize = 0;
	User *pUsers = new User[maxSize];

	/*User::add_user(&pUsers, maxSize, currentSize, "Petras", 2023, 30.03);*/
	User::read_file(filePath1, &pUsers, maxSize, currentSize);

	for (int i = 0; i < currentSize; i++) {
		User::print_user(&pUsers[i]);
		/*User removedUser = pUsers[i].remove_user();*/
	}


	delete[] pUsers;
	return 0;
}
