#include "userResult.h"

UserResult::UserResult() : user_(User()), hash_("") {}

UserResult::UserResult(User user) : user_(user), hash_("") {}

UserResult::UserResult(User user, string hash) : user_(user), hash_(hash) {}

UserResult::~UserResult() {}

User UserResult::get_user() {
	return user_;
}

string UserResult::get_hash() {
	return hash_;
}

void UserResult::set_user(User newUser) {
	user_ = newUser;
}

void UserResult::set_hash(string hash) {
	UserResult::hash_ = hash;
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

bool UserResult::check_hash_ends_with_a_number() {
	char lastCharacter = hash_.back();
	if (isdigit(lastCharacter)) {
		return true;
	}
	return false;
}

void UserResult::print_user_result() {
	printf("Name: %-20s Year: %10d DayMonth: %10.2lf Hash: %-64s\n",
		user_.get_name().c_str(), user_.get_year(), user_.get_day_month(), hash_.c_str());
}
