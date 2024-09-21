#ifndef USER_RESULT_H
#define USER_RESULT_H

#include <cstdio>
#include <sstream>
#include <string>
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"
#include "cryptopp/filters.h"
#include "user.h"
#include "user.h"

using namespace std;

class UserResult {
private:
	User user_;
	string hash_;

public:
	UserResult();
	UserResult(User user);
	UserResult(User user, string hash);
	~UserResult();

	User get_user();
	string get_hash();

	void set_user(User newUser);
	void set_hash(string hash);
	
	string generate_sha256();
	bool check_hash_ends_with_a_number();

	void print_user_result();
};

#endif
