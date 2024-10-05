#include <cstdio>
#include <sstream>
#include <regex.h>
#include "../rapidjson/document.h"
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/stringbuffer.h"
#include "../cryptopp/sha.h"
#include "../cryptopp/blake2.h"
#include "../cryptopp/hex.h"
#include "../cryptopp/filters.h"

using namespace std;
using namespace rapidjson;

const char *NAMES[] = { "Jonas", "Petras", "Antanas", "Juozas", "Stasys", "Ruta", "Rugile", "Greta", "Anastasija", "Ona", "Bulve" };

void fill_array(Value &usersArray, Document::AllocatorType &allocator, const float &VALID_USERS_PERCENTAGE, const int &ARRAY_SIZE);
bool check_hash_ends_with_a_number(string hash);
std::string generate_sha256(string message);
std::string generate_blake2b(string message);
string generate_string(string name, int year, double dayMonth);

int main(int args, char *arg[]) {
	if (args != 4) {
		printf("Usage: %s <filePath> <percentage> <arraySize>\n", arg[0]);
		return 1;
	}
	const char *FILE_PATH = arg[1];
	const float VALID_USERS_PERCENTAGE = atof(arg[2]);
	const int ARRAY_SIZE = atof(arg[3]);
	if (VALID_USERS_PERCENTAGE < 0 || VALID_USERS_PERCENTAGE > 1) {
		printf("Percentage must be between 0 and 1.\n");
		return 1;
	}
	FILE *pFile = fopen(FILE_PATH, "w");
	if (pFile == NULL) {
		printf("Error opening the file");
		return 1;
	}

	Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	Value usersArray(kArrayType);
	fill_array(usersArray, allocator, VALID_USERS_PERCENTAGE, ARRAY_SIZE);
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

void fill_array(Value &usersArray, Document::AllocatorType &allocator, const float &VALID_USERS_PERCENTAGE, const int &ARRAY_SIZE) {
	int usersAdded = 0;
	for (int i = 0; usersAdded < ARRAY_SIZE; i++) {
		Value userObject(kObjectType);

		int year = 1980 + i % 42;
		double dayMonth = 1 + i % 12;
		if (dayMonth == 2) {
			dayMonth = (dayMonth / 100) + (1 + (i % ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28)));
		} else if (dayMonth == 4 || dayMonth == 6 || dayMonth == 9 || dayMonth == 11) {
			dayMonth = (dayMonth / 100) + (1 + (i % 30));
		} else {
			dayMonth = (dayMonth / 100) + (1 + (i % 31));
		}
		string name = NAMES[i % (sizeof(NAMES) / sizeof(NAMES[0]))];
		Value nameValue(NAMES[i % (sizeof(NAMES) / sizeof(NAMES[0]))], allocator);

		string hash = generate_blake2b(generate_sha256(generate_string(name, year, dayMonth)));
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

std::string generate_sha256(string message) {
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

std::string generate_blake2b(string message) {
	CryptoPP::BLAKE2b hash;;
	CryptoPP::byte digest[CryptoPP::BLAKE2b::DIGESTSIZE];
	hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(message.c_str()), message.length());

	CryptoPP::HexEncoder encoder;
	string hashOutput;
	encoder.Attach(new CryptoPP::StringSink(hashOutput));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return hashOutput;
}

string generate_string(string name, int year, double dayMonth) {
	string nameReverse = name;
	reverse(nameReverse.begin(), nameReverse.end());
	stringstream stream;
	stream << name << (year * dayMonth) << nameReverse;

	regex_t regex;
	regcomp(&regex, "[A-Za-z0-9!@#$%^&*()_\\-\\+={}\'\",.<>?`~]", REG_EXTENDED);
	string message = stream.str();
	string result;
	char previous = message[0];
	for (int i = 0; result.length() < 16; i++) {
		char c = message[i % message.length()];
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
			result += c;
		}
		previous = c | message[i % message.length()];
	}

	return result;
}

bool check_hash_ends_with_a_number(string hash) {
	char lastCharacter = hash.back();
	if (isdigit(lastCharacter)) {
		return true;
	}
	return false;
}
