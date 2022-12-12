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
inline constexpr uint8_t priority(const char c) {
	return c - 'A' > 26 ? c + 1 - 'a'
	                    : c + 27 - 'A';
}

int main(void) {
	uint32_t priority_sum = 0;
	string line;
	string_view line_view;

#if 0
	while(getline(cin, line)) {
		assert(!(line.size() & 1));
		line_view = line;

		const string::size_type half = line_view.size() >> 1;
		for(const char c1 : line_view.substr(0, half))
			for(const char c2 : line_view.substr(half))
				if(c1 == c2) {
					priority_sum += priority(c1);
					goto breaker;
				}
breaker:
		;
	}
#else
	uint64_t upper, lower;
	while(getline(cin, line)) {
		assert(!(line.size() & 1));
		line_view = line;
		upper = 0;
		lower = 0;

		const string::size_type half = line_view.size() >> 1;
		for(const char& c : line_view.substr(0, half))
			lower |= 1ull << (c - 'A');
		for(const char& c : line_view.substr(half))
			upper |= 1ull << (c - 'A');

		const uint64_t res_bits = lower & upper;
		assert(popcount(res_bits) == 1);
		priority_sum += priority('A' + countr_zero(res_bits));
	}
#endif

	cout << priority_sum << '\n';
	return 0;
}
