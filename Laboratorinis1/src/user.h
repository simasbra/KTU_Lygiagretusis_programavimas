#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
	std::string name_;
	int year_;
	double dayMonth_;

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

	bool isValid();

	static void print_user(User &user);
};

#endif
