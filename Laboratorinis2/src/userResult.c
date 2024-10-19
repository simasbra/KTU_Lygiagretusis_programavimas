#include "userResult.h"
#include <ctype.h>
#include <math.h>
#include <regex.h>
#include <unistd.h>
#include <string.h>

void UR_generate_string(UserResult *pUserResult, char *pOutput[]) {
	if (!pUserResult) return;
	regex_t regex;
	regcomp(&regex, "[A-Za-z0-9!@#$%^&*()_\\-\\+={}\'\",.<>?`~]", REG_EXTENDED);

	User userTemporary = pUserResult->user;
	int nameLenght = strlen(userTemporary.name);
	char nameReversed[nameLenght];
	strcpy(nameReversed, userTemporary.name);
	string_inplace_reverse(nameReversed);

	int multiplication = userTemporary.year * userTemporary.dayMonth;
	int messageLenght = (int)((ceil(log10(multiplication))) * sizeof(char)) + nameLenght * 2 + 1;
	char message[messageLenght];
	sprintf(message, "%s%d%s", userTemporary.name, multiplication, nameReversed);

	char result[16];
	char previous = message[0];
	int resultLenght = 0;

	for (int i = 0; resultLenght < 16; i++) {
		char c = message[i % messageLenght];
		switch(i % 5) {
			case 0:
				c = c | (previous & c);
				break;
			case 1:
				c = c ^ previous;
				break;
			case 2:
				c = c & previous;
				break;
			case 3:
				c = !c;
				break;
			default:
				break;
		}
		char charStr[2] = { c, '\0' };
		if (regexec(&regex, charStr, 0, NULL, 0) == 0) {
			result[++resultLenght] += c;
			resultLenght++;
		}
		previous = c | message[i % messageLenght];
	}
	*pOutput = result;
}

void	UR_hash_using_sha256(char *pMessage[], char *pHash[]);
void	UR_hash_using_blake2b(char *pMessage[], char *pHash[]);

int UR_check_has_ends_with_a_number(UserResult *pUserResult) {
	if (!pUserResult) return 0;
	if (isdigit(pUserResult->hash[127])) {
		return 1;
	}
	return 0;
}

void UR_print_user_result(UserResult *pUserResult){
	if (!pUserResult) return;
	printf("| %-20s | %10d | %10.2lf | %-128s |\n",
	pUserResult->user.name, pUserResult->user.year, pUserResult->user.dayMonth, pUserResult->hash);
}

void UR_print_user_result_to_file(UserResult *pUserResult, FILE *pFile) {
	if (!pUserResult) return;
	fprintf(pFile, "| %-20s | %10d | %10.2lf | %-128s |\n",
	 pUserResult->user.name, pUserResult->user.year, pUserResult->user.dayMonth, pUserResult->hash);
}

void string_inplace_reverse(char *pString) {
	if (!pString) return;
	char *pEnd = pString + strlen(pString) + 1;
	// Swap values in two given variables
	#define XOR_SWAP(a, b) do\
	{\
		a ^= b;\
		b ^= a;\
		a ^= b;\
	} while(0);
	// Walk inwards from both ends of the string, swapping until the middle
	while (pString < pEnd) {
		XOR_SWAP(*pString, *pEnd);
		pString++;
		pEnd--;
	}
	#undef XOR_SWAP
}
