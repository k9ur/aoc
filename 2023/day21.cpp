#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <cassert>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<string> grid;
	string line;
	size_t start_r, start_c;
	bool found_S = false;

	while(getline(cin, line)) {
		grid.push_back(line);
		assert(line.size() == grid.front().size());

		const auto S_idx = line.find('S');
		if(S_idx != string::npos) {
			start_r = grid.size() - 1;
			start_c = S_idx;
			found_S = true;
		}
	}
	assert(found_S);

	set<pair<size_t, size_t>> positions;
	positions.emplace(start_r, start_c);

	for(uint8_t st = 0; st != 64; ++st) {
		const auto positions_copy = positions;
		for(const auto& [ pos_r, pos_c ] : positions_copy) {
			positions.erase({ pos_r, pos_c });
			if(pos_r && grid[pos_r - 1][pos_c] != '#')
				positions.emplace(pos_r - 1, pos_c);
			if(pos_r != grid.size() - 1 && grid[pos_r + 1][pos_c] != '#')
				positions.emplace(pos_r + 1, pos_c);
			if(pos_c && grid[pos_r][pos_c - 1] != '#')
				positions.emplace(pos_r, pos_c - 1);
			if(pos_c != grid.front().size() - 1 && grid[pos_r][pos_c + 1] != '#')
				positions.emplace(pos_r, pos_c + 1);
		}
	}

	cout << positions.size() << '\n';
	return 0;
}

