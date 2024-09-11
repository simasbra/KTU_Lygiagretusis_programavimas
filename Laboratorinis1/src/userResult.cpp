#include <cstdio>
#include <sstream>
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"
#include "cryptopp/filters.h"
#include "userResult.h"

using namespace std;

UserResult::UserResult() : user(User()), hash("") {}

UserResult::UserResult(User user, string hash) : user(user), hash(hash) {}

UserResult::~UserResult() {}

User UserResult::get_user() {
	return user;
}

string UserResult::get_hash() {
	return hash;
}

void UserResult::set_user(User newUser) {
	user = newUser;
}

void UserResult::set_hash(string hash) {
	UserResult::hash = hash;
}

void UserResult::print_user_result(UserResult &userResult) {
	printf("Name: %s, Year: %d, DayMonth: %.2lf, Hash: %s\n",
		userResult.user.get_name().c_str(), userResult.user.get_year(), userResult.user.get_day_month(), userResult.get_hash().c_str());
}

string UserResult::generate_sha256() {
	User user = UserResult::get_user();
	stringstream stream;
	stream << user.get_name() << (user.get_year() * user.get_day_month());
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
