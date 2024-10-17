#ifndef USER_RESULT_H
#define USER_RESULT_H

#include "user.h"

typedef struct UserResult {
	char hash[128];
	User user;
} UserResult;

void UR_print_user_result(UserResult *pUserResult);

/*UserResult();*/
/*UserResult(User user);*/
/*UserResult(User user, string hash);*/
/*~UserResult();*/
/**/
/*User get_user();*/
/*string get_hash();*/
/**/
/*void set_user(User newUser);*/
/*void set_hash(string newHash);*/
/*void set_sum(double newSum);*/
/**/
/*string hash_using_sha256(string message);*/
/*string hash_using_blake2b(string message);*/
/*bool check_hash_ends_with_a_number();*/
/**/
/*void print_user_result();*/
/*void print_user_result_to_file(FILE *pFile);*/
/*string generate_string();*/

#endif

