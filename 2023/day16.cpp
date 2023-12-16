#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cassert>
#include <cstdint>

using namespace std;

enum class dir_t : uint8_t
{
	UP, RIGHT, DOWN, LEFT
};
using enum dir_t;

struct Beam
{
	size_t r, c;
	dir_t dir;

	constexpr Beam() {}
	constexpr Beam(size_t _r, size_t _c, dir_t _dir)
	  : r(_r)
	  , c(_c)
	  , dir(_dir)
	{}

	constexpr void add_next(queue<Beam>& q, vector<string>& grid, vector<vector<bool>>& energised, size_t& energised_count) noexcept
	{
		if(dir == UP || dir == DOWN) {
			if(r == (dir == UP ? 0 : grid.size() - 1))
				return;
			vector<bool>::reference b = energised[dir == UP ? --r : ++r][c];
			if(!b) {
				++energised_count;
				b = true;
			}
			auto& ch = grid[r][c];
			if(ch == 'X')
				return;
			else if(ch == '/')
				dir = dir == UP ? RIGHT : LEFT;
			else if(ch == '\\')
				dir = dir == UP ? LEFT : RIGHT;
			else if(ch == '-') {
				dir = RIGHT;
				q.emplace(r, c, LEFT);
				ch = 'X'; // Prevent loops
			}
		} else {
			if(c == (dir == LEFT ? 0 : grid.front().size() - 1))
				return;
			vector<bool>::reference b = energised[r][dir == LEFT ? --c : ++c];
			if(!b) {
				++energised_count;
				b = true;
			}
			auto& ch = grid[r][c];
			if(ch == 'X')
				return;
			else if(ch == '/')
				dir = dir == LEFT ? DOWN : UP;
			else if(ch == '\\')
				dir = dir == LEFT ? UP : DOWN;
			else if(ch == '|') {
				dir = DOWN;
				q.emplace(r, c, UP);
				ch = 'X'; // Prevent loops
			}
		}
		q.emplace(r, c, dir);
	}
};

int main(void)
{
	vector<string> grid;
	string line;

	while(getline(cin, line)) {
		grid.push_back(line);
		assert(line.size() == grid.front().size());
	}

	vector<vector<bool>> energised(grid.size(), vector<bool>(grid.front().size(), false));

	queue<Beam> q;
	energised[0][0] = true;
	auto& sc = grid[0][0];
	q.emplace(0, 0, sc == '/' ? UP : sc == '\\' || sc == '|' ? DOWN : RIGHT);
	if(sc == '|')
		sc = 'X'; // Prevent loops

	size_t energised_count = 1;
	do {
		auto beam = q.front();
		q.pop();
		beam.add_next(q, grid, energised, energised_count);
	} while(!q.empty());

	cout << energised_count << '\n';
	return 0;
}

