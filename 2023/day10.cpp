#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <cassert>
#include <cstdint>

using namespace std;

using pos_t = pair<size_t, size_t>;

#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool conn_d(const char c)
{
	return c == '|' || c == 'F' || c == '7';
}
#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool conn_u(const char c)
{
	return c == '|' || c == 'J' || c == 'L';
}
#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool conn_l(const char c)
{
	return c == '-' || c == 'J' || c == '7';
}
#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool conn_r(const char c)
{
	return c == '-' || c == 'F' || c == 'L';
}

void get_next(const vector<string>& grid, pos_t& path, pos_t& prev)
{
	auto& [ r, c ] = path;
	const auto [ prev_r, prev_c ] = prev;
	const auto cols = grid.front().size(),
	           rows = grid.size();
	const auto cur = grid[r][c];
	prev = { r, c };
	if(r && prev_r != r - 1 && conn_u(cur) && conn_d(grid[r - 1][c]))
		--r;
	else if(r != rows - 1 && prev_r != r + 1 && conn_d(cur) && conn_u(grid[r + 1][c]))
		++r;
	else if(c && prev_c != c - 1 && conn_l(cur) && conn_r(grid[r][c - 1]))
		--c;
	else if(c != cols - 1 && prev_c != c + 1 && conn_r(cur) && conn_l(grid[r][c + 1]))
		++c;
	else
		exit(EXIT_FAILURE);
}

int main(void)
{
	vector<string> grid;
	string line;
	size_t start_r = 0;
	auto start_c = string::npos;

	while(getline(cin, line)) {
		grid.push_back(line);
		if(start_c == string::npos) {
			start_c = line.find_first_of('S');
			if(start_c == string::npos) // Still wasn't it
				++start_r;
		}
	}
	assert(start_c != string::npos);
	const auto cols = grid.front().size(),
	           rows = grid.size();

	array<pos_t, 2> paths, prevs; // Only the most recent of the path
	prevs.fill({ start_r, start_c });
	size_t arr_i = 0;
	if(start_r && conn_d(grid[start_r - 1][start_c]))
		paths[arr_i++] = { start_r - 1, start_c };
	if(start_r != rows - 1 && conn_u(grid[start_r + 1][start_c]))
		paths[arr_i++] = { start_r + 1, start_c };
	if(start_c && conn_r(grid[start_r][start_c - 1]))
		paths[arr_i++] = { start_r, start_c - 1 };
	if(start_c != cols - 1 && conn_l(grid[start_r][start_c + 1]))
		paths[arr_i++] = { start_r, start_c + 1 };
	assert(arr_i == 2);

	uint32_t steps = 1;
	do {
		get_next(grid, paths[0], prevs[0]);
		get_next(grid, paths[1], prevs[1]);
		++steps;
	} while(paths[0] != paths[1]);

	cout << steps << '\n';
	return 0;
}

