#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <cstdint>

using namespace std;

int main(void)
{
	string s;
	uint32_t mul_sum = 0;

	while (getline(cin, s)) {
		const regex mul_regex("mul\\(\\d{1,3},\\d{1,3}\\)");
		const sregex_iterator words_begin(s.cbegin(), s.cend(), mul_regex), words_end{};
		for (auto it = words_begin; it != words_end; ++it) {
			istringstream iss(it->str());
			uint16_t X, Y;
			iss.ignore(4);
			iss >> X;
			iss.ignore(1);
			iss >> Y;
			mul_sum += X * Y;
		}
	}

	cout << mul_sum << '\n';
	return 0;
}

