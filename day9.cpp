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

inline constexpr void set_cell(vector<vector<bool>>& grid, uint32_t& visited, const size_t x, const size_t y) {
	if(!grid[y][x]) {
		++visited;
		grid[y][x] = true;
	}
}

void constexpr do_instruction(vector<vector<bool>>& grid, uint32_t& visited, const Instruction* const ins, const bool x, size_t& T_axis, size_t& T_other, size_t& H_axis, const size_t& H_other) {
	uint32_t moved = 0;
	while(T_other != H_other && moved < ins->moves) {
		++moved;
		ins->dir & 2 ? --H_axis : ++H_axis;
		if(abs(static_cast<int>(H_axis) - static_cast<int>(T_axis)) == 2) {
			T_other = H_other;
			T_axis = ins->dir & 2 ? H_axis + 1 : H_axis - 1;
			set_cell(grid, visited, x ? T_axis : T_other, x ? T_other : T_axis);
			break;
		}
	}
	ins->dir & 2 ? H_axis -= ins->moves - moved : H_axis += ins->moves - moved;
	while(ins->dir & 2 ? T_axis > H_axis + 1 : T_axis < H_axis - 1) {
		set_cell(grid, visited, x ? T_axis : T_other, x ? T_other : T_axis);
		ins->dir & 2 ? --T_axis : ++T_axis;
	}
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

	size_t H_x = -min_x,
	       H_y = -min_y,
	       T_x = H_x,
	       T_y = H_y;
	vector<vector<bool>> grid(max_y - min_y + 1);
	for(vector<bool>& row : grid)
		row.resize(max_x - min_x + 1);

	uint32_t visited = 0;
	for(const Instruction* const ins : instructions) {
		if(ins->dir & 1) // y-axis
			do_instruction(grid, visited, ins, 0, T_y, T_x, H_y, H_x);
		else // x-axis
			do_instruction(grid, visited, ins, 1, T_x, T_y, H_x, H_y);
		set_cell(grid, visited, T_x, T_y);
	}

	cout << visited << '\n';
	return 0;
}
