#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <cstdint>

using namespace std;

int main(void)
{
	uint32_t mul_sum = 0;
	uint16_t X, Y;
	string s;

	while (getline(cin, s)) {
		regex mul_regex("mul\\(\\d{1,3},\\d{1,3}\\)");
		sregex_iterator words_begin(s.cbegin(), s.cend(), mul_regex), words_end{};
		for (auto it = words_begin; it != words_end; ++it) {
			istringstream iss(it->str());
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

