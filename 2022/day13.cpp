#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <cctype>
#include <cstdint>

using namespace std;

constexpr char list_open = '[',
               list_close = ']';

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

template<integral T>
T process_num(const string& str, string::size_type& ind)
{
	assert(isdigit(str[ind]));
	const auto old_ind = ind;
	do
		++ind;
	while(ind < str.size() && isdigit(str[ind]));
	const string_view view = str;
	return svto<T>(view.substr(old_ind, ind - old_ind));
}

constexpr void conv_to_list(string& str, string::size_type& ind)
{
	str.insert(ind, 1, list_close);
	do
		--ind;
	while(isdigit(str[ind]));
	++ind;
}

bool in_right_order(string& left, string& right)
{
	uint32_t l_num, r_num;
	string::size_type l_i{}, r_i{};

	while(l_i < left.size() && r_i < right.size()) {
		uint8_t l_state, r_state;
		switch(left[l_i]) {
			case list_open:
				l_state = 0;
				++l_i;
				break;
			case list_close: // Left list should close first
				l_state = 1;
				++l_i;
				if(right[r_i] != list_close) // Left closed before right
					return true;
				else { // Closed at same time
					++r_i;
					break;
				}
			default:
				l_state = 2;
				l_num = process_num<uint32_t>(left, l_i);
		}

		if(l_state != 1)
			switch(right[r_i]) {
				case list_open:
					r_state = 0;
					++r_i;
					break;
				case list_close: // Right list shouldn't close first
					return false;
				default:
					r_state = 2;
					r_num = process_num<uint32_t>(right, r_i);
			}

		if(l_state == 2 && r_state == 2) { // Both nums
			if(l_num > r_num)
				return false;
			else if(l_num < r_num)
				return true;
		} else if(l_state == 0 && r_state == 2)
			conv_to_list(right, r_i);
		else if(l_state == 2 && r_state == 0)
			conv_to_list(left, l_i);

		if(left[l_i] == ',')
			++l_i;
		if(right[r_i] == ',')
			++r_i;
	}
	return true;
}

int main(void)
{
	uint32_t index = 1, index_sum{};
	string left, right;

	while(getline(cin, left)) {
		assert(getline(cin, right));
		
		if(in_right_order(left, right))
			index_sum += index;
		if(!cin.eof())
			getline(cin, left); // Just ignore blank line
		++index;
	}

	cout << index_sum << '\n';
	return 0;
}

