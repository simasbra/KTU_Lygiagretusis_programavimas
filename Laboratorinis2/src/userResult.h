#ifndef USER_RESULT_H
#define USER_RESULT_H

#include "user.h"
#include <stdio.h>

typedef struct UserResult {
	User	user;
	char	hash[128];
} UserResult;

void	UR_generate_string		(UserResult *pUserResult, char *pOutput);
void	UR_hash_using_sha256		(char *pMessage, char *pHash);
void	UR_hash_using_blake2b		(char *pMessage, char *pHash);

int	UR_check_hash_ends_with_a_number(UserResult *pUserResult);
void	UR_print_user_result		(UserResult *pUserResult);
void	UR_print_user_result_to_file	(UserResult *pUserResult, FILE *pFile);

void	string_inplace_reverse		(char *pString);

#endif
