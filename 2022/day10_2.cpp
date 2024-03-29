#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <array>
#include <cstdint>

using namespace std;

constexpr size_t width = 40,
                 rows = 6;

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

template<size_t N>
constexpr void update_cycle(uint32_t& cycle, array<string, N>& screen, const int32_t X)
{
	if(abs(X - static_cast<int32_t>(cycle % width)) <= 1)
		screen[cycle / width][cycle % width] = '#';
	++cycle;
}

int main(void) {
	array<string, rows> screen;
	string line;
	size_t cycles_i = 0;
	uint32_t cycle = 0;
	int32_t sum{}, X = 1;

	screen.fill(string(width, '.'));

	while(getline(cin, line)) {
		if(line[0] == 'n')
			update_cycle(cycle, screen, X);
		else {
			update_cycle(cycle, screen, X);
			update_cycle(cycle, screen, X);
			X += svto<int32_t>(string_view(line).substr(5));
		}
	}

	for(const auto& str : screen)
		cout << str << '\n';

	return 0;
}

