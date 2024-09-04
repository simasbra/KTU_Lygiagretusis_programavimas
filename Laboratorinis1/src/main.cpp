#include "user.h"

using namespace std;

int main(void) {
	const int maxSize = 10;
	int currentSize = 0;
	User *users = new User[maxSize];

	User::add_user(&users, &maxSize, &currentSize, "Petras", 2023, 30.03);

	for (int i = 0; i < currentSize; i++) {
		User::print_user(&users[i]);
	}

	return 0;
}
