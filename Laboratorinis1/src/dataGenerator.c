#include <stdlib.h>
#include "cjson/cJSON.h"

void fill_array(cJSON *);

int main(void) {
	cJSON *pUsers = cJSON_CreateArray();

	cJSON_free(pUsers);
	return 0;
}

void fill_array(cJSON *pUsers) {
	const int size = 25;
	char *names[] = { "Jonas", "Petras", "Antanas", "Juozas", "Stasys" };

	for (int i = 0; i < size; i++) {

	}
}
