#ifndef USER_RESULT_H
#define USER_RESULT_H

#include <string>
#include "user.h"

class UserResult {
private:
	User user_;
	std::string hash_;

public:
	UserResult();
	UserResult(User user);
	UserResult(User user, std::string hash);
	~UserResult();

	User get_user();
	std::string get_hash();

	void set_user(User newUser);
	void set_hash(std::string hash);
	
	std::string generate_sha256();
	bool check_hash_ends_with_a_number();

	static void print_user_result(UserResult &userResult);
};

#endif