#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <charconv>
#include <concepts>
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
	constexpr array<const uint32_t, 3> limits = { 12, 13, 14 }; // r g b order
	uint32_t ID_sum = 0;
	string line;
	string_view line_view;

	while(getline(cin, line)) {
		string_view::size_type i;
		line_view = line;

		for(i = 6; line_view[i] != ':'; ++i);
		const auto ID = svto<uint32_t>(line_view.substr(5, i - 5));
		i += 2;

		bool poss = true;
		do {
			const auto old_i = i;
			do
				++i;
			while(line_view[i] != ' ');
			const auto count = svto<uint32_t>(line_view.substr(old_i, i - old_i));
			const auto first_l = line_view[++i];
			const size_t lim_i = first_l == 'r' ? 0 : first_l == 'g' ? 1 : 2;
			if(count > limits[lim_i]) {
				poss = false;
				break;
			}
			i += 4;
			while(i < line_view.size() && line_view[i] != ' ')
				++i;
		} while(++i < line_view.size());

		if(poss)
			ID_sum += ID;
	}

	cout << ID_sum << '\n';
	return 0;
}

