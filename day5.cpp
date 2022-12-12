#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <cassert>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
constexpr T svto(const string_view& sv) {
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

int main(void) {
	vector<string> pos;
	string line, msg;
	string_view line_view;

	bool got_size = false;
	while(getline(cin, line) && !line.empty()) {
		if(!got_size) {
			assert(line.size() % 4 == 3);
			pos.resize((line.size() + 1) / 4);
			got_size = true;
		}
		for(size_t i = 0; i < line.size(); i += 4)
			if(line[i] == '[')
				pos[i / 4].push_back(line[i + 1]);
	}
	for(string& s : pos)
		reverse(s.begin(), s.end());
	
	while(getline(cin, line)) { // Move
		string_view::size_type i;
		line_view = line;

		for(i = 6; line_view[i] != ' '; ++i);
		const size_t count = svto<size_t>(line_view.substr(5, i - 5));
		const string_view::size_type old_i = i + 6;

		for(i += 7; line_view[i] != ' '; ++i);
		const size_t from = svto<size_t>(line_view.substr(old_i, i - old_i)) - 1,
		               to = svto<size_t>(line_view.substr(i + 4, line_view.size() - i - 3)) - 1;

		assert(pos[from].size() >= count);
#if 0
		string move_str = pos[from].substr(pos[from].size() - count, count);
		reverse(move_str.begin(), move_str.end());
		pos[to] += move_str;
#else
		const ranges::subrange move_str(pos[from].crbegin(), pos[from].crbegin() + count);
		ranges::copy(move_str, back_inserter(pos[to]));
#endif
		pos[from].erase(pos[from].size() - count);
	}

	for(const string& s : pos)
		msg.push_back(s.back());

	cout << msg << '\n';
	return 0;
}
