#ifndef USER_RESULT_H
#define USER_RESULT_H

#include "user.h"
#include <stdio.h>

typedef struct UserResult {
	char	hash[128];
	User	user;
} UserResult;

void	UR_hash_using_sha256(char *pMessage[], char *pHash[]);
void	UR_hash_using_blake2b(char *pMessage[], char *pHash[]);
void	UR_generate_string(UserResult *pUserResult, char *pOutput[]);
int	UR_check_has_ends_with_a_number(UserResult *pUserResult);
void	UR_print_user_result(UserResult *pUserResult);
void	UR_print_user_result_to_file(UserResult *pUserResult, FILE *pFile);

#endif

