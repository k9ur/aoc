#include <iostream>
#include <string>
#include <string_view>
#include <cassert>
#include <bit>
#include <cstdint>

using namespace std;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr uint8_t priority(const char c)
{
	return c - 'A' > 26 ? c + 1 - 'a'
	                    : c + 27 - 'A';
}

int main(void)
{
	string line;
	string_view line_view;
	uint64_t upper, lower;
	uint32_t priority_sum = 0;

	while(getline(cin, line)) {
		assert(!(line.size() & 1));
		line_view = line;
		upper = 0;
		lower = 0;

		const auto half = line_view.size() >> 1;
		for(const auto c : line_view.substr(0, half))
			lower |= 1ull << (c - 'A');
		for(const auto c : line_view.substr(half))
			upper |= 1ull << (c - 'A');

		const auto res_bits = lower & upper;
		assert(popcount(res_bits) == 1);
		priority_sum += priority('A' + countr_zero(res_bits));
	}

	cout << priority_sum << '\n';
	return 0;
}

