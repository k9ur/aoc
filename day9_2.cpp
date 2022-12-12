#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <vector>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
constexpr T svto(const string_view& sv) {
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

struct Instruction {
	const uint32_t moves;
	const uint8_t dir : 2;

	Instruction(const uint32_t _moves, const uint8_t _dir) : moves(_moves), dir(_dir) {}
};

struct Point {
	size_t y;
	size_t x;
};

inline constexpr void set_cell(vector<vector<bool>>& grid, uint32_t& visited, const size_t x, const size_t y) {
	if(!grid[y][x]) {
		++visited;
		grid[y][x] = true;
	}
}

inline constexpr void update_axis(const size_t prev, size_t& current) {
	if(prev > current)
		++current;
	else if(prev < current)
		--current;
}

int main(void) {
	vector<Instruction*> instructions;
	string line;
	string_view line_view;

	int32_t x = 0,
	        y = 0;
	int32_t max_x = 0,
	        min_x = 0,
	        max_y = 0,
	        min_y = 0;
	while(getline(cin, line)) {
		line_view = line;
		const uint32_t num = svto<uint32_t>(line_view.substr(2));
		if(!num)
			continue;
		switch(line[0]) {
			case 'R':
				x += num;
				if(x > max_x)
					max_x = x;
				instructions.push_back(new Instruction(num, 0));
				break;
			case 'U':
				y += num;
				if(y > max_y)
					max_y = y;
				instructions.push_back(new Instruction(num, 1));
				break;
			case 'L':
				x -= num;
				if(x < min_x)
					min_x = x;
				instructions.push_back(new Instruction(num, 2));
				break;
			case 'D':
				y -= num;
				if(y < min_y)
					min_y = y;
				instructions.push_back(new Instruction(num, 1 | 2));
				break;
			default:
				exit(EXIT_FAILURE);
		}
	}

	vector<vector<bool>> grid(max_y - min_y + 1);
	for(vector<bool>& row : grid)
		row.resize(max_x - min_x + 1);
	grid[-min_y][-min_x] = true;

	uint32_t visited = 1;
	constexpr size_t length = 10;
	Point chain[length];
	for(Point& point : chain) {
		point.x = -min_x;
		point.y = -min_y;
	}

	Point& head = chain[0],
	       &tail = chain[length - 1];
	for(const Instruction* const ins : instructions)
		for(uint32_t m = 0; m != ins->moves; ++m) {
			if(ins->dir & 1) // y-axis
				ins->dir & 2 ? --head.y : ++head.y;
			else // x-axis
				ins->dir & 2 ? --head.x : ++head.x;

			size_t i;
			for(i = 1; i != length; ++i) {
				Point& current = chain[i];
				const Point& prev = chain[i - 1];

				if(abs(static_cast<int>(current.x) - static_cast<int>(prev.x)) == 2
					|| abs(static_cast<int>(current.y) - static_cast<int>(prev.y)) == 2) {
					update_axis(prev.x, current.x);
					update_axis(prev.y, current.y);
				} else
					break;
			}
			if(i == length) // Tail moved
				set_cell(grid, visited, tail.x, tail.y);
		}

	cout << visited << '\n';
	return 0;
}
