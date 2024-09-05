#include "user.h"

using namespace std;

int main(void) {
	const int maxSize = 10;
	int currentSize = 0;
	User *pUsers = new User[maxSize];

	User::add_user(&pUsers, maxSize, currentSize, "Petras", 2023, 30.03);

	for (int i = 0; i < currentSize; i++) {
		User::print_user(&pUsers[i]);
	}

	delete[] pUsers;
	return 0;
}
