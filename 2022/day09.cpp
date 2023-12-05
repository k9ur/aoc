#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <vector>
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

constexpr void set_cell(vector<vector<bool>>& grid, uint32_t& visited, const size_t x, const size_t y) {
	if(!grid[y][x]) {
		++visited;
		grid[y][x] = true;
	}
}

struct Instruction {
	const uint32_t moves;
	const uint8_t dir : 2;

	Instruction(uint32_t _moves, uint8_t _dir)
	  : moves(_moves)
	  , dir(_dir)
	{}

	void constexpr execute(vector<vector<bool>>& grid, uint32_t& visited, const bool x, size_t& T_axis, size_t& T_other, size_t& H_axis, const size_t& H_other) const noexcept {
		uint32_t moved = 0;
		while(T_other != H_other && moved < moves) {
			++moved;
			dir & 2 ? --H_axis : ++H_axis;
			if(abs(static_cast<int32_t>(H_axis) - static_cast<int32_t>(T_axis)) == 2) {
				T_other = H_other;
				T_axis = dir & 2 ? H_axis + 1 : H_axis - 1;
				set_cell(grid, visited, x ? T_axis : T_other, x ? T_other : T_axis);
				break;
			}
		}
		dir & 2 ? H_axis -= moves - moved : H_axis += moves - moved;
		while(dir & 2 ? T_axis > H_axis + 1 : T_axis < H_axis - 1) {
			set_cell(grid, visited, x ? T_axis : T_other, x ? T_other : T_axis);
			dir & 2 ? --T_axis : ++T_axis;
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

	size_t H_x = -min_x,
	       H_y = -min_y,
	       T_x = H_x,
	       T_y = H_y;
	vector<vector<bool>> grid(max_y - min_y + 1);
	for(vector<bool>& row : grid)
		row.resize(max_x - min_x + 1);

	uint32_t visited = 0;
	for(unique_ptr<Instruction>& ins : instructions) {
		if(ins->dir & 1) // y-axis
			ins->execute(grid, visited, 0, T_y, T_x, H_y, H_x);
		else // x-axis
			ins->execute(grid, visited, 1, T_x, T_y, H_x, H_y);
		set_cell(grid, visited, T_x, T_y);
	}

	cout << visited << '\n';
	return 0;
}
