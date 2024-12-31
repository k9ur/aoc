#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ranges>
#include <cstdint>

using namespace std;

bool test(const vector<uint32_t>& vals, const uint64_t test_val)
{
	const uint32_t limit = 1u << (vals.size() - 1);

	for (uint32_t bits_copy, bits = 0; bits != limit; ++bits) {
		uint64_t ongoing_val = vals.front();
		bits_copy = bits;
		for (const auto val : vals | views::drop(1)) {
			bits_copy & 1
				? ongoing_val += val
				: ongoing_val *= val;
			if (ongoing_val > test_val)
				break;
			bits_copy >>= 1;
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

		vals.clear();
	}

	cout << valid_sum << '\n';
	return 0;
}

