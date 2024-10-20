#include "userResult.h"
#include "include/crypto-algorithms/sha256.h"
#include <ctype.h>
#include <math.h>
#include <regex.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

void UR_generate_string(UserResult *pUserResult, char *pOutput) {
	if (!pUserResult) return;
	regex_t regex;
	regcomp(&regex, "[A-Za-z0-9!@#$%^&*()_\\-\\+={}\'\",.<>?`~]", REG_EXTENDED);

	User userTemporary = pUserResult->user;
	int nameLenght = strlen(userTemporary.name);
	char nameReversed[nameLenght + 1];
	strcpy(nameReversed, userTemporary.name);
	string_inplace_reverse(nameReversed);

	int multiplication = userTemporary.year * userTemporary.dayMonth;
	int messageLenght = (int)((ceil(log10(multiplication))) * sizeof(char)) + nameLenght * 2 + 1;
	char message[messageLenght];
	sprintf(message, "%s%d%s", userTemporary.name, multiplication, nameReversed);

	char result[16];
	char previous = message[0];
	int resultLength = 0;

	for (int i = 0; resultLength < 16; i++) {
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
			result[++resultLength] += c;
			resultLength++;
			if (resultLength >= 16) break;
		}
		previous = c | message[i % messageLenght];
	}
	strncpy(pOutput, result, 16);
	pOutput[15] = '\0';
}

void UR_hash_using_sha256(char *pMessage, char *pHash[32]) {
	// Buffer to store the hash (BYTE array)
	BYTE hash[SHA256_BLOCK_SIZE];
	// Initialize the SHA-256 context
	SHA256_CTX ctx;
	sha256_init(&ctx);
	// Update the context with the input string
	sha256_update(&ctx, (const BYTE*)pMessage, strlen(pMessage));
	// Finalize the hash computation
	sha256_final(&ctx, hash);
	// Copy the computed hash to the output buffer
	memcpy(pHash, hash, SHA256_BLOCK_SIZE);
}

/*void UR_hash_using_blake2b(char *pMessage[16], char *pHash[128]) {*/
/**/
/*}*/

int UR_check_hash_ends_with_a_number(UserResult *pUserResult) {
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
