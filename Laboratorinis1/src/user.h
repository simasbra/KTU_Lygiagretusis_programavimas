#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
	std::string name;
	int year;
	double dayMonth;

public:
	User();
	User(std::string name, int year, double dayMonth);

	std::string getName();
	int getYear();
	double getDayMonth();

	void setName(std::string name);
	void setYear(int year);
	void setDayMonth(double dayMonth);

	void printUser(User *user);
	void addUser(User *users[], int *maxSize, int *currentSize, std::string name, int year, double dayMonth);

};

#endif
