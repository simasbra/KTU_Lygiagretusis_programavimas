#ifndef USER_H
#define USER_H

typedef struct User {
	double dayMonth;
	int year;
	int nameLength;
	char name[];
} User;

User U_new(int yearNew, double dayMonthNew, char nameNew[]);
int U_is_valid(User *pUser);
void U_print_user(User *pUser);

#endif
