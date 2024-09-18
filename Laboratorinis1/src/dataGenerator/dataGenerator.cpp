#include <cstdio>
#include <sstream>
#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/stringbuffer.h"
#include "../cryptopp/sha.h"
#include "../cryptopp/hex.h"
#include "../cryptopp/filters.h"

using namespace std;
using namespace rapidjson;

const char FILE_PATH[] = "./data/IFF22_BradaitisV_L1_dat_3.json";

const float VALID_USERS_PERCENTAGE = 0;
const int ARRAY_SIZE = 100;
const char *NAMES[] = { "Jonas", "Petras", "Antanas", "Juozas", "Stasys", "Ruta", "Rugile", "Greta", "Anastasija", "Ona" };

void fill_array(Value &usersArray, Document::AllocatorType &allocator);
bool check_hash_ends_with_a_number(string hash);
std::string generate_sha256(string name, int year, double dayMonth);

int main(void) {
	FILE *pFile = fopen(FILE_PATH, "w");
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
	document.AddMember("usersCount", ARRAY_SIZE, allocator);
	
	// converting document to json
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	document.Accept(writer);

	fprintf(pFile, "%s", buffer.GetString());
	fclose(pFile);

	return 0;
}

void fill_array(Value &usersArray, Document::AllocatorType &allocator) {
	int usersAdded = 0;
	for (int i = 0; usersAdded < ARRAY_SIZE; i++) {
		Value userObject(kObjectType);

		int year = 1990 + i % 32;
		double dayMonth = (1 + i % 31) + ((1 + i % 12) / 100.0);
		string name = NAMES[i % (sizeof(NAMES) / sizeof(NAMES[0]))];
		Value nameValue(NAMES[i % (sizeof(NAMES) / sizeof(NAMES[0]))], allocator);

		string hash = generate_sha256(name, year, dayMonth);
		if (!check_hash_ends_with_a_number(hash) && usersAdded < ARRAY_SIZE * VALID_USERS_PERCENTAGE) {
			userObject.AddMember("name", nameValue, allocator);
			userObject.AddMember("year", year, allocator);
			userObject.AddMember("dayMonth", dayMonth, allocator);

			usersArray.PushBack(userObject, allocator);
			usersAdded++;
		}
		else if (check_hash_ends_with_a_number(hash) && usersAdded >= ARRAY_SIZE * VALID_USERS_PERCENTAGE) {
			userObject.AddMember("name", nameValue, allocator);
			userObject.AddMember("year", year, allocator);
			userObject.AddMember("dayMonth", dayMonth, allocator);

			usersArray.PushBack(userObject, allocator);
			usersAdded++;
		}
	}
}

std::string generate_sha256(string name, int year, double dayMonth) {
	std::stringstream stream;
	stream << name << (year * dayMonth);
	string message = stream.str();

	CryptoPP::SHA256 hash;
	CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
	hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(message.c_str()), message.length());

	CryptoPP::HexEncoder encoder;
	string hashOutput;
	encoder.Attach(new CryptoPP::StringSink(hashOutput));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return hashOutput;
}

bool check_hash_ends_with_a_number(string hash) {
	char lastCharacter = hash.back();
	if (isdigit(lastCharacter)) {
		return true;
	}
	return false;
}
