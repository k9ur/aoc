#include <iostream>
#include <string>
#include <cassert>
#include <stdint.h>

using namespace std;

#ifdef gnu
[[using gnu : const, always_inline, hot]]
#endif
static inline constexpr uint8_t priority(const char& c) {
	return c - 'A' > 26 ? c + 1 - 'a'
	                    : c + 27 - 'A';
}

int main(void) {
	uint32_t priority_sum = 0;
	string line;

#if 0
	while(getline(cin, line)) {
		assert(!(line.size() & 1));

		const string::size_type half = line.size() >> 1;
		for(const char c1 : line.substr(0, half))
			for(const char c2 : line.substr(half))
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
		upper = 0;
		lower = 0;

		const string::size_type half = line.size() >> 1;
		for(const char& c : line.substr(0, half))
			lower |= 1ull << (c - 'A');
		for(const char& c : line.substr(half))
			upper |= 1ull << (c - 'A');

		const uint64_t res_bits = lower & upper;
		assert(res_bits >> __builtin_ctzll(res_bits) == 1);
		priority_sum += priority('A' + __builtin_ctzll(res_bits));
	}
#endif

	cout << priority_sum << '\n';
	return 0;
}
