#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <cmath>
#include <cassert>
#include <cstdint>

using namespace std;

struct State
{
	using Tile = pair<size_t, size_t>; // r, c
	enum Direction : uint8_t {
		NORTH, EAST, SOUTH, WEST
	};

	Tile tile;
	uint32_t score = 0;
	Direction dir;

	constexpr State() {}
	constexpr State(const Tile& _tile, const uint32_t _score, const Direction _dir)
	  : tile(_tile)
	  , score(_score)
	  , dir(_dir)
	{}

	constexpr Tile get_next_tile(void) const noexcept
	{
		const auto& [ r, c ] = tile;
		switch (dir) {
			case Direction::NORTH:
				return { r - 1, c };
			case Direction::EAST:
				return { r, c + 1 };
			case Direction::SOUTH:
				return { r + 1, c };
			case Direction::WEST:
				return { r, c - 1 };
			default:
				break;
		}
		return {};
	}
	void add_score(const uint32_t added = 1) noexcept
	{
		score += added;
	}
	template<bool clockwise>
	void rotate_dir(const uint8_t times = 1) noexcept
	{
		if constexpr (clockwise)
			dir = static_cast<Direction>((dir + times) % 4);
		else
			dir = static_cast<Direction>(((static_cast<int8_t>(dir) - times) % 4 + 4) % 4); // positive modulus
	}
	constexpr uint8_t dir_difference(const State& other) const noexcept
	{
		return abs(static_cast<int8_t>(dir) - static_cast<int8_t>(other.dir));
	}

	bool been_here_before(const vector<State>& visited) const noexcept
	{
		bool been = false;
		for (const auto& before : visited)
			if (tile == before.tile) // use >= not > since it's faster
				if ((dir_difference(before) == 0 && score >= before.score)
				||  (dir_difference(before) == 1 && score >= before.score + 1'000)
				||  (dir_difference(before) == 2 && score >= before.score + 2'000)) {
					been = true;
					break;
				}
		return been;
	}
};

struct Maze : vector<string>
{
	char at_tile(const State::Tile& tile) const noexcept
	{
		return (*this)[tile.first][tile.second];
	}

	constexpr size_t rows(void) const noexcept
	{
		return size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return front().size();
	}
};

template<char end>
uint32_t solve_bfs(const Maze& maze, State&& start_state)
{
	queue<State> states;
	vector<State> visited;
	uint32_t min_score = 0;

	states.push(move(start_state));

	do {
		auto& state = states.front();

		if (maze.at_tile(state.tile) == end) {
			if (!min_score || min_score > state.score)
				min_score = state.score;
		} else if (!state.been_here_before(visited)) { // haven't been here before, or have been here with a higher score
			visited.push_back(state);

			state.add_score();
			State::Tile next_tile = state.get_next_tile();
			if (maze.at_tile(next_tile) != '#')
				states.emplace(next_tile, state.score, state.dir);

			state.add_score(1'000);
			state.rotate_dir<false>(1); // turn left
			next_tile = state.get_next_tile();
			if (maze.at_tile(next_tile) != '#')
				states.emplace(next_tile, state.score, state.dir);
			state.rotate_dir<true>(2); // turn right
			next_tile = state.get_next_tile();
			if (maze.at_tile(next_tile) != '#')
				states.emplace(next_tile, state.score, state.dir);
		}

		states.pop();
	} while (!states.empty());

	assert(min_score);
	return min_score;
}

int main(void)
{
	constexpr char start = 'S',
	               end = 'E';

	Maze maze{};
	State::Tile tile;

	{
		string s;
		while (getline(cin, s)) {
			const auto s_idx = s.find(start);
			if (s_idx != string::npos) {
				tile.second = s_idx;
				tile.first = maze.rows();
			}
			maze.push_back(s);
			assert(s.size() == maze.cols());
		}
	}

	cout << solve_bfs<end>(maze, { tile, 0, State::Direction::EAST }) << '\n';
	return 0;
}

