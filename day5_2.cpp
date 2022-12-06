#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ranges>
#include <stdint.h>

using namespace std;

int main(void) {
	vector<string> init, pos;
	string line, msg;

	char c;
	string::size_type i;
	while(getline(cin, line)) {
		if(line.empty()) [[unlikely]]
			continue;

		else if(line[0] == 'm') [[likely]] { // Move
			for(i = 6; line[i] != ' '; ++i);
			const size_t count = stoi(line.substr(5, i - 5));
			string::size_type old_i = i + 6;

			for(i += 7; line[i] != ' '; ++i);
			const size_t from = stoi(line.substr(old_i, i - old_i)) - 1,
			               to = stoi(line.substr(i + 4, line.size() - i - 3)) - 1;

			assert(pos[from].size() >= count);
			pos[to] += pos[from].substr(pos[from].size() - count, count);
			pos[from].resize(pos[from].size() - count);

		} else if(line[1] < '0' || line[1] > '9') [[likely]] { // Starting information
			init.push_back(line);
		
		} else [[unlikely]] { // Row numbers
			i = line.size() - 3;
			while(line[i] != ' ')
				--i;

			const size_t rows = stoi(line.substr(i + 1, line.size() - i - 2));
			pos.resize(rows);

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
