#ifndef USER_H
#define USER_H

#include <cstdio>
#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace std;

class User {
private:
	string name_;
	int year_;
	double dayMonth_;

public:
	User();
	User(string name, int year, double dayMonth);
	~User();

	string get_name();
	int get_year();
	double get_day_month();

	void set_name(string name);
	void set_year(int year);
	void set_day_month(double dayMonth);

	bool is_valid();
	void print_user();

	static User get_user_from_value(rapidjson::Value &userValue);
};

#endif
