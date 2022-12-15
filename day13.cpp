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
T svto(const string_view& sv) {
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

template<integral T>
T process_num(const string_view& view, string_view::size_type& ind) {
	assert(isdigit(view[ind]));
	const string_view::size_type old_ind = ind;
	do
		++ind;
	while(ind < view.size() && isdigit(view[ind]));
	return svto<T>(view.substr(old_ind, ind - old_ind));
}

inline constexpr void conv_to_list(string& str, string_view& str_view, string_view::size_type& ind) {
	str.insert(ind, 1, list_close);
	str_view = str;
	do
		--ind;
	while(isdigit(str_view[ind]));
	++ind;
}

bool in_right_order(string& left, string& right) {
	uint32_t l_num, r_num;
	string_view left_view = left,
	            right_view = right;
	string_view::size_type l_ind = 0,
	                       r_ind = 0;

	while(l_ind < left_view.size() && r_ind < right_view.size()) {
		uint8_t l_state, r_state;
		switch(left_view[l_ind]) {
			case list_open:
				l_state = 0;
				++l_ind;
				break;
			case list_close: // Left list should close first
				l_state = 1;
				++l_ind;
				if(right_view[r_ind] != list_close) // Left closed before right
					return true;
				else { // Closed at same time
					++r_ind;
					break;
				}
			default:
				l_state = 2;
				l_num = process_num<uint32_t>(left_view, l_ind);
		}

		if(l_state != 1)
			switch(right_view[r_ind]) {
				case list_open:
					r_state = 0;
					++r_ind;
					break;
				case list_close: // Right list shouldn't close first
					return false;
				default:
					r_state = 2;
					r_num = process_num<uint32_t>(right_view, r_ind);
			}

		if(l_state == 2 && r_state == 2) { // Both nums
			if(l_num > r_num)
				return false;
			else if(l_num < r_num)
				return true;
		} else if(l_state == 0 && r_state == 2)
			conv_to_list(right, right_view, r_ind);
		else if(l_state == 2 && r_state == 0)
			conv_to_list(left, left_view, l_ind);

		if(left_view[l_ind] == ',')
			++l_ind;
		if(right_view[r_ind] == ',')
			++r_ind;
	}
	return true;
}

int main(void) {
	uint32_t index = 1,
	         index_sum = 0;
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
