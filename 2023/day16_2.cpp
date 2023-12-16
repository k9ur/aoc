#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
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

	static constexpr void add_first(queue<Beam>& q, vector<string>& grid, const size_t r, const size_t c, const dir_t dir)
	{ // add_next should maybe be add_current to prevent this duplicate code
		auto& sc = grid[r][c];
		if(dir == RIGHT || dir == LEFT) {
			if(sc == '|') {
				sc = 'X';
				q.emplace(r, c, UP);
				q.emplace(r, c, DOWN);
			} else
				q.emplace(r, c, sc == '/' ? (dir == RIGHT ? UP : DOWN) : sc == '\\' ? (dir == RIGHT ? DOWN : UP) : dir);
		} else {
			if(sc == '-') {
				sc = 'X';
				q.emplace(r, c, LEFT);
				q.emplace(r, c, RIGHT);
			} else
				q.emplace(r, c, sc == '/' ? (dir == UP ? RIGHT : LEFT) : sc == '\\' ? (dir == UP ? LEFT : RIGHT) : dir);
		}
	}

	static constexpr void test_params(vector<string> grid, size_t& max_e_count, const size_t r, const size_t c, const dir_t dir)
	{
		queue<Beam> q;
		vector<vector<bool>> energised(grid.size(), vector<bool>(grid.front().size(), false));
		energised[r][c] = true;
		Beam::add_first(q, grid, r, c, dir);

		size_t energised_count = 1;
		do {
			auto beam = q.front();
			q.pop();
			beam.add_next(q, grid, energised, energised_count);
		} while(!q.empty());
		max_e_count = max(max_e_count, energised_count);
	}

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

	size_t max_e_count = 0;
	for(size_t r = 0; r != grid.size(); ++r) {
		Beam::test_params(grid, max_e_count, r, 0, RIGHT);
		Beam::test_params(grid, max_e_count, r, grid.front().size() - 1, LEFT);
	}
	for(size_t c = 0; c != grid.front().size(); ++c) {
		Beam::test_params(grid, max_e_count, 0, c, DOWN);
		Beam::test_params(grid, max_e_count, grid.size() - 1, c, UP);
	}

	cout << max_e_count << '\n';
	return 0;
}

