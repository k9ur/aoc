#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <bitset>
#include <queue>
#include <utility>
#include <cstdint>

using namespace std;

using Position = pair<uint8_t, uint8_t>; // r, c

template<size_t rows, size_t cols>
void bfs_add(queue<Position>& q, array<bitset<cols>, rows>& visited, const array<bitset<cols>, rows>& corrupted, const uint8_t r, const uint8_t c, const bool cond = true)
{
	if (cond && !visited[r][c] && !corrupted[r][c]) {
		q.emplace(r, c);
		visited[r][c] = true;
	}
}

template<size_t rows, size_t cols>
bool can_solve_bfs(const array<bitset<cols>, rows>& corrupted)
{
	queue<Position> q;
	array<bitset<cols>, rows> visited;

	bfs_add(q, visited, corrupted, 0, 0);
	do {
		const auto& [ r, c ] = q.front();

		if (r == rows - 1 && c == cols - 1)
			return true;
		bfs_add(q, visited, corrupted, r - 1, c, r);
		bfs_add(q, visited, corrupted, r, c - 1, c);
		bfs_add(q, visited, corrupted, r + 1, c, r != rows - 1);
		bfs_add(q, visited, corrupted, r, c + 1, c != cols - 1);

		q.pop();
	} while (!q.empty());

	return false;
}

int main(void)
{
	constexpr uint16_t start_at = 1'024 + 1;
	constexpr uint8_t rows = 71,
	                  cols = 71;
	static_assert(rows && cols);

	array<bitset<cols>, rows> corrupted;
	uint16_t r{}, c{}; // uint8_t is unsigned char unfortunately

	{
		string s;
		uint16_t count = 0;
		bool found_ans = false;

		while (getline(cin, s))
			if (!found_ans) {
				istringstream iss(s);

				iss >> r;
				iss.ignore(1);
				iss >> c;
				corrupted[r][c] = true;

				if (count != start_at)
					++count;
				else if (!can_solve_bfs(corrupted))
					found_ans = true;
			}
	}

	cout << r << ',' << c << '\n';
	return 0;
}

