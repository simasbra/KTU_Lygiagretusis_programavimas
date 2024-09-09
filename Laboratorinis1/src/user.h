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
	~User();

	std::string get_name();
	int get_year();
	double get_day_month();

	void set_name(std::string name);
	void set_year(int year);
	void set_day_month(double dayMonth);

	static void print_user(User *pUser);
	static void add_user(User *pUsers[], const int &maxSize, int &currentSize, std::string name, int year, double dayMonth);
	User remove_user();

	static void read_file(std::string filePath);
};

#endif
