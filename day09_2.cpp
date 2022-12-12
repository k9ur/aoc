#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <vector>
#include <array>
#include <memory>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
T svto(const string_view& sv) {
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

struct Point {
	size_t x;
	size_t y;

	Point() {}
	Point(size_t _x, size_t _y) : x(_x), y(_y) {}
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

struct Instruction {
	const uint32_t moves;
	const uint8_t dir : 2;

	static constexpr size_t length = 10;

	Instruction(uint32_t _moves, uint8_t _dir) : moves(_moves), dir(_dir) {}

	void execute(array<Point, length>& chain, vector<vector<bool>>& grid, uint32_t& visited) const noexcept {
		static Point& head = chain.front(),
	                  &tail = chain.back();
		for(uint32_t m = 0; m != moves; ++m) {
			if(dir & 1) // y-axis
				dir & 2 ? --head.y : ++head.y;
			else // x-axis
				dir & 2 ? --head.x : ++head.x;

			size_t i;
			for(i = 1; i != length; ++i) {
				Point& current = chain[i];
				const Point& prev = chain[i - 1];

				if(abs(static_cast<int32_t>(current.x) - static_cast<int32_t>(prev.x)) == 2
					|| abs(static_cast<int32_t>(current.y) - static_cast<int32_t>(prev.y)) == 2) {
					update_axis(prev.x, current.x);
					update_axis(prev.y, current.y);
				} else
					break;
			}
			if(i == length) // Tail moved
				set_cell(grid, visited, tail.x, tail.y);
		}
	}
};

void add_instruction(vector<unique_ptr<Instruction>>& instructions, int32_t& xy, int32_t& maxmin_xy, const uint32_t num, const uint8_t dir) {
	if(dir & 2) {
		xy -= num;
		if(xy < maxmin_xy)
			maxmin_xy = xy;
	} else {
		xy += num;
		if(xy > maxmin_xy)
			maxmin_xy = xy;
	}
	instructions.push_back(make_unique<Instruction>(num, dir));
}

int main(void) {
	vector<unique_ptr<Instruction>> instructions;
	string line;

	int32_t x = 0,
	        y = 0;
	int32_t max_x = 0,
	        min_x = 0,
	        max_y = 0,
	        min_y = 0;
	while(getline(cin, line)) {
		const uint32_t num = svto<uint32_t>(string_view(line).substr(2));
		if(!num)
			continue;
		switch(line[0]) {
			case 'R':
				add_instruction(instructions, x, max_x, num, 0);
				break;
			case 'U':
				add_instruction(instructions, y, max_y, num, 1);
				break;
			case 'L':
				add_instruction(instructions, x, min_x, num, 2);
				break;
			case 'D':
				add_instruction(instructions, y, min_y, num, 1 | 2);
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
	array<Point, Instruction::length> chain;
	chain.fill(Point(-min_x, -min_y));

	for(unique_ptr<Instruction>& ins : instructions)
		ins->execute(chain, grid, visited);

	cout << visited << '\n';
	return 0;
}
