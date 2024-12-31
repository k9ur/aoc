#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <ranges>
#include <cmath>
#include <cstdint>

using namespace std;

constexpr uint8_t digits(uint64_t num)
{
	uint8_t res = 1;
	while (num /= 10)
		++res;
	return res;
}

bool test(const vector<uint32_t>& vals, const uint64_t test_val)
{
	constexpr array<uint32_t, 10> ten_pows = { 1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000 };

	const uint32_t limit = pow(3, vals.size() - 1);

	for (uint32_t terns_copy, terns = 0; terns != limit; ++terns) {
		uint64_t ongoing_val = vals.front();
		terns_copy = terns;
		for (const auto val : vals | views::drop(1)) {
			switch (terns_copy % 3) {
				case 0:
					ongoing_val += val;
					break;
				case 1:
					ongoing_val *= val;
					break;
				case 2:
					ongoing_val *= ten_pows[digits(val)];
					ongoing_val += val;
					break;
				default:
					break;
			}
			if (ongoing_val > test_val)
				break;
			terns_copy /= 3;
		}
		if (ongoing_val == test_val)
			return true;
	}
	return false;
}

int main(void)
{
	string s;
	uint64_t valid_sum = 0;

	while (getline(cin, s)) {
		vector<uint32_t> vals;
		istringstream iss(s);
		uint64_t test_val;
		uint32_t val;

		iss >> test_val;
		iss.ignore(1);
		while (iss >> val)
			vals.push_back(val);

		if (test(vals, test_val))
			valid_sum += test_val;

		iss.clear();
		vals.clear();
	}

	cout << valid_sum << '\n';
	return 0;
}

