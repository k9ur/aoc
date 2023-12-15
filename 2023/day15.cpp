#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr uint8_t hash_func(const string& s)
{
	uint16_t val = 0;
	for(const auto c : s) {
		val += c;
		val *= 17;
		val %= 256;
	}
	return val;
}

int main(void)
{
	string line, seq;
	uint32_t hash_sum = 0;

	getline(cin, line);
	istringstream iss(line);
	while(getline(iss, seq, ','))
		hash_sum += hash_func(seq);

	cout << hash_sum << '\n';
	return 0;
}

