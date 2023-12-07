#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <charconv>
#include <concepts>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

int main(void)
{
	array<uint32_t, 3> min_needed = { 0 };
	uint32_t pow_sum = 0;
	string line;
	string_view line_view;

	while(getline(cin, line)) {
		string_view::size_type i;
		line_view = line;

		for(i = 6; line_view[i] != ':'; ++i);
		i += 2;

		do {
			const auto old_i = i;
			do
				++i;
			while(line_view[i] != ' ');
			const auto count = svto<uint32_t>(line_view.substr(old_i, i - old_i));
			const auto first_l = line_view[++i];
			const size_t arr_i = first_l == 'r' ? 0 : first_l == 'g' ? 1 : 2;
			min_needed[arr_i] = max(min_needed[arr_i], count);
			i += 4;
			while(i < line_view.size() && line_view[i] != ' ')
				++i;
		} while(++i < line_view.size());

		pow_sum += min_needed[0] * min_needed[1] * min_needed[2];
		min_needed.fill(0);
	}

	cout << pow_sum << '\n';
	return 0;
}

