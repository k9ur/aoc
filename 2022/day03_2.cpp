#include <iostream>
#include <string>
#include <cassert>
#include <numeric>
#include <array>
#include <bit>
#include <functional>
#include <execution>
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
	constexpr size_t size = 3;
	string line;
	array<uint64_t, size> line_bits;
	size_t count = 0;
	uint32_t priority_sum = 0;

	while(getline(cin, line)) {
		line_bits[count] = 0;
		for(const auto c : line)
			line_bits[count] |= 1ull << (c - 'A');

		if(++count != size)
			continue;
		count = 0;

		const auto res_bits = reduce(execution::par_unseq, next(line_bits.cbegin()), line_bits.cend(), line_bits.front(), bit_and<>());
		assert(popcount(res_bits) == 1);
		priority_sum += priority('A' + countr_zero(res_bits));
	}
	assert(count == 0);

	cout << priority_sum << '\n';
	return 0;
}

