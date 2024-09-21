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

string UserResult::hash_using_sha256() {
	string message = generate_string();
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

string UserResult::hash_using_blake2b() {
	string message = generate_string();
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

bool UserResult::check_hash_ends_with_a_number() {
	char lastCharacter = hash_.back();
	if (isdigit(lastCharacter)) {
		return true;
	}
	return false;
}

void UserResult::print_user_result() {
	printf("Name: %-15s Year: %4d DayMonth: %5.2lf Hash: %-128s\n",
		user_.get_name().c_str(), user_.get_year(), user_.get_day_month(), hash_.c_str());
}

string UserResult::generate_string() {
	User user = UserResult::get_user();
	string nameReverse = user.get_name();
	reverse(nameReverse.begin(), nameReverse.end());
	stringstream stream;
	stream << user.get_name() << (user.get_year() * user.get_day_month()) << nameReverse;

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
