#include "user.h"
#include <stdio.h>
#include <string.h>

User U_new(int yearNew, double dayMonthNew, char nameNew[]) {
	User user;
	user.year = yearNew;
	user.dayMonth = dayMonthNew;
	strcpy(user.name, nameNew);
	return user;
}

int User_is_valid(User *pUser) {
	if (pUser == NULL) {
		return 0;
	}
	if (pUser->year > 0 && pUser->dayMonth > 0 && pUser->name[0] != '\0') {
		return 1;
	}
	return 0;
}

void User_print_user(User *pUser) {
	printf("| %-20s | %10d | %10.2lf |\n", pUser->name, pUser->year, pUser->dayMonth);
}
