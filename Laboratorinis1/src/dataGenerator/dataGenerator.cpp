#include <cstdio>
#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/stringbuffer.h"

using namespace rapidjson;

const char filePath1[] = "./data/IFF22_BradaitisV_L1_dat_1.json";

const int arraySize = 100;
const char *names[] = { "Jonas", "Petras", "Antanas", "Juozas", "Stasys" };

void fill_array(Value &usersArray, Document::AllocatorType &allocator);

int main(void) {
	FILE *pFile = fopen(filePath1, "w");
	if (pFile == NULL) {
		printf("Error opening the file");
		return 1;
	}

	Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	Value usersArray(kArrayType);
	fill_array(usersArray, allocator);
	document.AddMember("users", usersArray, allocator);
	document.AddMember("usersCount", arraySize, allocator);
	
	// converting document to json
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	document.Accept(writer);

	fprintf(pFile, "%s", buffer.GetString());
	fclose(pFile);

	return 0;
}

void fill_array(Value &usersArray, Document::AllocatorType &allocator) {
	for (int i = 0; i < arraySize; i++) {
		Value userObject(kObjectType);

		int year = 1990 + i % 30;
		double dayMonth = (1 + i % 31) + ((1 + i % 12) / 100.0);

		Value name(names[i % (sizeof(names) / sizeof(names[0]))], allocator);
		userObject.AddMember("name", name, allocator);
		userObject.AddMember("year", year, allocator);
		userObject.AddMember("dayMonth", dayMonth, allocator);

		usersArray.PushBack(userObject, allocator);
	}
}
