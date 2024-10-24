#ifndef USER_H
#define USER_H

typedef struct {
	double	dayMonth;
	int	year;
	char	name[32];
} User;

User	U_new		(int yearNew, double dayMonthNew, char nameNew[]);
int	U_is_valid	(User *pUser);
void	U_print_user	(User *pUser);

#endif
