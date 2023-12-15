#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <algorithm>
#include <charconv>
#include <concepts>
#include <vector>
#include <memory>
#include <cstdint>

using namespace std;

template<integral T, int base = 10>
T svto(const string_view& sv)
{
	T val;
	[[maybe_unused]] const errc ec = from_chars(sv.cbegin(), sv.cend(), val, base).ec;
	assert(ec == errc());
	return val;
}

constexpr void set_cell(vector<bool>::reference&& cell, uint32_t& visited)
{
	if(!cell) {
		++visited;
		cell = true;
	}
}

struct Instruction
{
	const uint32_t moves;
	const uint8_t dir : 2;

	constexpr Instruction(uint32_t _moves, uint8_t _dir)
	  : moves(_moves)
	  , dir(_dir)
	{}

	void constexpr execute(vector<vector<bool>>& grid, uint32_t& visited, const bool x, size_t& T_axis, size_t& T_other, size_t& H_axis, const size_t& H_other) const noexcept
	{
		uint32_t moved = 0;
		while(T_other != H_other && moved < moves) {
			++moved;
			dir & 2 ? --H_axis : ++H_axis;
			if(abs(static_cast<int32_t>(H_axis) - static_cast<int32_t>(T_axis)) == 2) {
				T_other = H_other;
				T_axis = dir & 2 ? H_axis + 1 : H_axis - 1;
				set_cell(grid[x ? T_other : T_axis][x ? T_axis : T_other], visited);
				break;
			}
		}
		dir & 2 ? H_axis -= moves - moved : H_axis += moves - moved;
		while(dir & 2 ? T_axis > H_axis + 1 : T_axis < H_axis - 1) {
			set_cell(grid[x ? T_other : T_axis][x ? T_axis : T_other], visited);
			dir & 2 ? --T_axis : ++T_axis;
		}
	}
};

void add_instruction(vector<unique_ptr<Instruction>>& instructions, int32_t& xy, int32_t& maxmin_xy, const uint32_t num, const uint8_t dir)
{
	if(dir & 2) {
		xy -= num;
		maxmin_xy = min(maxmin_xy, xy);
	} else {
		xy += num;
		maxmin_xy = max(maxmin_xy, xy);
	}
	instructions.push_back(make_unique<Instruction>(num, dir));
}

int main(void)
{
	vector<unique_ptr<Instruction>> instructions;
	string line;

	int32_t x{}, y{}, max_x{}, min_x{}, max_y{}, min_y{};
	while(getline(cin, line)) {
		const auto num = svto<uint32_t>(string_view(line).substr(2));
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
	vector<vector<bool>> grid(max_y - min_y + 1, vector<bool>(max_x - min_x + 1, false));

	uint32_t visited = 0;
	for(const auto& ins : instructions) {
		if(ins->dir & 1) // y-axis
			ins->execute(grid, visited, false, T_y, T_x, H_y, H_x);
		else // x-axis
			ins->execute(grid, visited, true, T_x, T_y, H_x, H_y);
		set_cell(grid[T_y][T_x], visited);
	}

	cout << visited << '\n';
	return 0;
}

