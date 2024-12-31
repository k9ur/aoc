#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <bitset>
#include <queue>
#include <tuple>
#include <cassert>
#include <cstdint>

using namespace std;

using State = tuple<uint16_t, uint8_t, uint8_t>; // steps, r, c

template<size_t rows, size_t cols>
void bfs_add(queue<State>& q, array<bitset<cols>, rows>& visited, const array<bitset<cols>, rows>& corrupted, const uint16_t steps, const uint8_t r, const uint8_t c, const bool cond = true)
{
	if (cond && !visited[r][c] && !corrupted[r][c]) {
		q.emplace(steps, r, c);
		visited[r][c] = true;
	}
}

template<size_t rows, size_t cols>
uint16_t solve_bfs(const array<bitset<cols>, rows>& corrupted)
{
	queue<State> q;
	array<bitset<cols>, rows> visited;
	uint16_t min_steps = 0;

	bfs_add(q, visited, corrupted, 0, 0, 0);
	do {
		const auto& [ steps, r, c ] = q.front();

		if (r == rows - 1 && c == cols - 1) {
			min_steps = steps;
			break;
		}
		bfs_add(q, visited, corrupted, steps + 1, r - 1, c, r);
		bfs_add(q, visited, corrupted, steps + 1, r, c - 1, c);
		bfs_add(q, visited, corrupted, steps + 1, r + 1, c, r != rows - 1);
		bfs_add(q, visited, corrupted, steps + 1, r, c + 1, c != cols - 1);

		q.pop();
	} while (!q.empty());

	assert(min_steps);
	return min_steps;
}

int main(void)
{
	constexpr uint16_t how_many = 1'024;
	constexpr uint8_t rows = 71,
	                  cols = 71;
	static_assert(rows && cols);

	array<bitset<cols>, rows> corrupted;

	{
		uint16_t count = 0;
		string s;
		while (getline(cin, s))
			if (count != how_many) {
				istringstream iss(s);
				uint16_t r, c; // uint8_t is unsigned char unfortunately

				iss >> r;
				iss.ignore(1);
				iss >> c;
				corrupted[r][c] = true;
				++count;
			}
	}

	cout << solve_bfs(corrupted) << '\n';
	return 0;
}

