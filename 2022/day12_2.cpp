#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <cstdint>

using namespace std;

constexpr char start_c = 'S',
               end_c = 'E';

#ifdef gnu
[[using gnu : const]]
#endif
constexpr bool is_destination(const char c) {
	return c == 'a' || c == start_c;
}

bool bfs_queue_add(queue<size_t>& q, const string& grid, vector<bool>& visited, const size_t pos, const char min_height) {
	if(!visited[pos]) {
		if(is_destination(grid[pos])) [[unlikely]]
			return min_height <= 'a';
		else if(grid[pos] >= min_height) [[likely]] {
			q.push(pos);
			visited[pos] = true;
		}
	}
	return false;
}

int main(void) {
	string grid, line;
	string::size_type size = 0;

	while(getline(cin, line)) {
		grid += line;
		if(size) [[likely]]
			assert(size == line.size());
		else [[unlikely]]
			assert(size = line.size());
	}

	vector<bool> visited(grid.size());
	queue<size_t> q;

	const string::size_type end_pos = grid.find(end_c);
	assert(end_pos != string::npos);
	q.push(end_pos);
	visited[end_pos] = true;

	uint32_t steps = 1;
	size_t step_incr = q.back();
	do { // bfs... backwards
		const size_t pos = q.front();
		const char min_height = (grid[pos] == end_c ? 'z' : grid[pos]) - 1;

		if(pos % size) // Left
			if(bfs_queue_add(q, grid, visited, pos - 1, min_height))
				break;
		if((pos + 1) % size) // Right
			if(bfs_queue_add(q, grid, visited, pos + 1, min_height))
				break;
		if(pos > size) // Above
			if(bfs_queue_add(q, grid, visited, pos - size, min_height))
				break;
		if(pos < grid.size() - size) // Below
			if(bfs_queue_add(q, grid, visited, pos + size, min_height))
				break;

		if(step_incr == pos) {
			++steps;
			step_incr = q.back();
		}
		q.pop();
	} while(!q.empty());

	cout << steps << '\n';
	return 0;
}
