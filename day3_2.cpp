#include <iostream>
#include <string>
#include <cassert>
#include <numeric>
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
	constexpr size_t size = 3;
	uint32_t priority_sum = 0,
	         count = 0;

#if 0
	string lines[size];
	while(getline(cin, lines[count++])) {
		if(count != size)
			continue;
		count = 0;

		for(const char c1 : lines[0]) {
			for(const char c2 : lines[1]) {
				if(c1 != c2)
					continue;
				for(const char c3 : lines[2])
					if(c1 == c3) {
						priority_sum += priority(c1);
						goto breaker;
					}
			}
		}
breaker:
		;
	}
#else
	string line;
	uint64_t line_bits[size];

	while(getline(cin, line)) {
		line_bits[count] = 0;
		for(const char& c : line)
			line_bits[count] |= 1ull << (c - 'A');

		if(++count != size)
			continue;
		count = 0;

		const uint64_t res_bits = accumulate(line_bits + 1, line_bits + size, line_bits[0], [](const uint64_t a, const uint64_t b) {
			return a & b;
		});
		assert(res_bits >> __builtin_ctzll(res_bits) == 1);
		priority_sum += priority('A' + __builtin_ctzll(res_bits));
	}
#endif
	assert(count == 0);

	cout << priority_sum << '\n';
	return 0;
}
