#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <set>
#include <algorithm>
#include <limits>
#include <functional>
#include <compare>
#include <cassert>
#include <cstdint>

#define DEBUG

using namespace std;

using point_t = pair<size_t, size_t>; // r, c

struct State
{
	enum class dir_t : uint8_t
	{
		NONE, UP, LEFT, DOWN, RIGHT
	};
	using enum dir_t;
	using distless_type = tuple<size_t, size_t, uint8_t, dir_t>; // r, c, count, dir

	point_t pos;
	size_t dist;
	uint8_t count;
	dir_t dir;

	constexpr State() {}
	constexpr State(size_t r, size_t c, size_t _dist, uint8_t _count, dir_t _dir)
	  : pos(r, c)
	  , dist(_dist)
	  , count(_count)
	  , dir(_dir)
	{}
	constexpr State(point_t _pos, size_t _dist = 0, uint8_t _count = 0, dir_t _dir = NONE)
	  : State(_pos.first, _pos.second, _dist, _count, _dir)
	{}

	constexpr strong_ordering operator<=>(const State& other) const noexcept
	{ // For std::greater
		return dist <=> other.dist;
	}
	constexpr vector<State> neighbours(const size_t rows, const size_t cols) const noexcept
	{ // Add all directions minus backwards, unless already traveled in same direction thrice
		vector<State> nbs;
		nbs.reserve(4);
		const auto [ r, c ] = pos;
		if(dir != DOWN && r && !(count == 10 && dir == UP) && !(count < 4 && dir != UP && dir != NONE))
			nbs.emplace_back(r - 1, c, dist, dir == UP ? count + 1 : 1, UP);
		if(dir != UP && r != rows - 1 && !(count == 10 && dir == DOWN) && !(count < 4 && dir != DOWN && dir != NONE))
			nbs.emplace_back(r + 1, c, dist, dir == DOWN ? count + 1 : 1, DOWN);
		if(dir != RIGHT && c && !(count == 10 && dir == LEFT) && !(count < 4 && dir != LEFT && dir != NONE))
			nbs.emplace_back(r, c - 1, dist, dir == LEFT ? count + 1 : 1, LEFT);
		if(dir != LEFT && c != cols - 1 && !(count == 10 && dir == RIGHT) && !(count < 4 && dir != RIGHT && dir != NONE))
			nbs.emplace_back(r, c + 1, dist, dir == RIGHT ? count + 1 : 1, RIGHT);
		return nbs; // .dist doesn't yet have the new tile dist added
	}
	constexpr distless_type distless(void) const noexcept
	{
		return { pos.first, pos.second, count, dir };
	}
};

constexpr size_t dijsktra(const vector<string>& dist_grid, const point_t& start, const point_t& end)
{
	priority_queue<State, vector<State>, greater<State>> pq;
	set<State::distless_type> visited;
	auto end_dist = numeric_limits<size_t>::max();

	pq.emplace(start);
	do {
		const auto u = pq.top();
		pq.pop();

		if(visited.contains(u.distless()))
			continue;
		if(u.pos == end)
			end_dist = min(end_dist, u.dist);
		visited.insert(u.distless());

		for(auto& v : u.neighbours(dist_grid.size(), dist_grid.front().size())) {
			v.dist += dist_grid[v.pos.first][v.pos.second] - '0';
			pq.push(v);
		}
	} while(!pq.empty());

	return end_dist;
}

int main(void)
{
	vector<string> grid;
	string line;

	while(getline(cin, line)) {
		grid.push_back(line);
		assert(line.size() == grid.front().size());
	}

	cout << dijsktra(grid, { 0, 0 }, { grid.size() - 1, grid.front().size() - 1 }) << '\n';
	return 0;
}

