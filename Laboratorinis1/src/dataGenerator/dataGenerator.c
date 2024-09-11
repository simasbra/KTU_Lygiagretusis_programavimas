#include <stdio.h>
#include <stdlib.h>
// https://github.com/DaveGamble/cJSON
#include "cjson/cJSON.h"

void fill_array(cJSON *);

int main(void) {
	cJSON *pUsers = cJSON_CreateArray();

	fill_array(pUsers);

	FILE *pFile = fopen("./data/Simas_BradaitisV_L1_dat_1.json", "w");
	if (pFile == NULL) {
		printf("Error opening the file");
		return 1;
	}

	char *pJsonData = cJSON_Print(pUsers);
	fprintf(pFile, "%s", pJsonData);

	cJSON_free(pUsers);
	fclose(pFile);

	return 0;
}

void fill_array(cJSON *pUsers) {
	const int size = 100;
	char *names[] = { "Jonas", "Petras", "Antanas", "Juozas", "Stasys" };

	for (int i = 0; i < size; i++) {
		cJSON *pUser = cJSON_CreateObject();
		int year = 1990 + i % 30;
		double dayMonth = (1 + i % 31) + ((1 + i % 12) / 100.0);

		cJSON_AddStringToObject(pUser, "name", names[i % (sizeof(names) / sizeof(names[0]))]);
		cJSON_AddNumberToObject(pUser, "year", year);
		cJSON_AddNumberToObject(pUser, "dayMonth", dayMonth);

		cJSON_AddItemToArray(pUsers, pUser);
	}
}
