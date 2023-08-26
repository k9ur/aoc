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

bool bfs_queue_add(queue<size_t>& q, const string& grid, vector<bool>& visited, const size_t pos, const char max_height) {
	if(!visited[pos]) {
		if(grid[pos] == end_c) [[unlikely]]
			return max_height >= 'z';
		else if(grid[pos] <= max_height) [[likely]] {
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

	const string::size_type start_pos = grid.find(start_c);
	assert(start_pos != string::npos);
	q.push(start_pos);
	visited[start_pos] = true;

	uint32_t steps = 1;
	size_t step_incr = q.back();
	do { // bfs
		const size_t pos = q.front();
		const char max_height = (grid[pos] == start_c ? 'a' : grid[pos]) + 1;

		if(pos % size) // Left
			if(bfs_queue_add(q, grid, visited, pos - 1, max_height))
				break;
		if((pos + 1) % size) // Right
			if(bfs_queue_add(q, grid, visited, pos + 1, max_height))
				break;
		if(pos > size) // Above
			if(bfs_queue_add(q, grid, visited, pos - size, max_height))
				break;
		if(pos < grid.size() - size) // Below
			if(bfs_queue_add(q, grid, visited, pos + size, max_height))
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
