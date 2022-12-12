#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <array>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
T svto(const string_view& sv) {
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

template<size_t N>
inline constexpr void update_cycle(uint32_t& cycle, const array<const uint32_t, N>& cycles, size_t& cycles_i, int32_t& sum, const int32_t X) {
	++cycle;
	if(cycle == cycles[cycles_i]) {
		sum += X * cycle;
		++cycles_i;
	}
}

int main(void) {
	constexpr array<const uint32_t, 6> cycles = { 20, 60, 100, 140, 180, 220 };
	size_t cycles_i = 0;
	uint32_t cycle = 0;
	int32_t sum = 0,
	        X = 1;

	string line;
	while(getline(cin, line)) {
		if(cycles_i == cycles.size()) // Continue to take input but ignore it
			continue;

		if(line[0] == 'n')
			update_cycle(cycle, cycles, cycles_i, sum, X);
		else {
			update_cycle(cycle, cycles, cycles_i, sum, X);
			update_cycle(cycle, cycles, cycles_i, sum, X);
			X += svto<int32_t>(string_view(line).substr(5));
		}
	}

	cout << sum << '\n';
	return 0;
}
