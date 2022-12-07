#include <iostream>
#include <string>
#include <string_view>
#include <cassert>
#include <vector>
#include <ranges>
#include <algorithm>
#include <cstdint>

using namespace std;

#ifdef gnu
[[using gnu : pure]]
#endif
static inline int svtoi(const string_view& str_view, size_t* idx = 0, int base = 10) {
	return stoi(static_cast<string>(str_view), idx, base);
}

int main(void) {
	vector<string> init, pos;
	string line, msg;
	string_view line_view;

	string_view::size_type i;
	while(getline(cin, line)) {
		if(line.empty()) [[unlikely]]
			continue;

		else if(line[0] == 'm') [[likely]] { // Move
			line_view = line;

			for(i = 6; line_view[i] != ' '; ++i);
			const size_t count = svtoi(line_view.substr(5, i - 5));
			const string_view::size_type old_i = i + 6;

			for(i += 7; line_view[i] != ' '; ++i);
			const size_t from = svtoi(line_view.substr(old_i, i - old_i)) - 1,
			               to = svtoi(line_view.substr(i + 4, line_view.size() - i - 3)) - 1;

			assert(pos[from].size() >= count);
			string move_str = pos[from].substr(pos[from].size() - count, count);
			reverse(move_str.begin(), move_str.end());
			pos[to] += move_str;
			pos[from].resize(pos[from].size() - count);

		} else if(line[1] < '0' || line[1] > '9') [[likely]] { // Starting information
			init.push_back(line);

		} else [[unlikely]] { // Row numbers
			line_view = line;

			i = line_view.size() - 3;
			while(line_view[i] != ' ')
				--i;

			const size_t rows = svtoi(line_view.substr(i + 1, line_view.size() - i - 2));
			pos.resize(rows);

			char c;
			for(const string& s : init | views::reverse)
				for(i = 0; i < rows; ++i)
					if((c = s[i * 4 + 1]) != ' ')
						pos[i].push_back(c);
			init.clear();
		}
			
	}

	for(const string& s : pos)
		msg.push_back(s.back());
	pos.clear();

	cout << msg << '\n';
	return 0;
}
