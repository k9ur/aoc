#include <iostream>
#include <cassert>
#include <string>
#include <string_view>
#include <charconv>
#include <concepts>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <cstdint>

using namespace std;

using point_t = pair<size_t, size_t>;

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

constexpr void update_axis(const size_t prev, size_t& current)
{
	if(prev > current)
		++current;
	else if(prev < current)
		--current;
}

struct Instruction
{
	const uint32_t moves;
	const uint8_t dir : 2;

	static constexpr size_t length = 10;

	constexpr Instruction(uint32_t _moves, uint8_t _dir)
	  : moves(_moves)
	  , dir(_dir)
	{}

	void execute(array<point_t, length>& chain, vector<vector<bool>>& grid, uint32_t& visited) const noexcept
	{
		static auto& head = chain.front();
		static auto& tail = chain.back();
		for(uint32_t m = 0; m != moves; ++m) {
			if(dir & 1) // y-axis
				dir & 2 ? --head.second : ++head.second;
			else // x-axis
				dir & 2 ? --head.first : ++head.first;

			size_t i;
			for(i = 1; i != length; ++i) {
				auto& current = chain[i];
				const auto& prev = chain[i - 1];

				if(abs(static_cast<int32_t>(current.first) - static_cast<int32_t>(prev.first)) == 2
					|| abs(static_cast<int32_t>(current.second) - static_cast<int32_t>(prev.second)) == 2) {
					update_axis(prev.first, current.first);
					update_axis(prev.second, current.second);
				} else
					break;
			}
			if(i == length) // Tail moved
				set_cell(grid[tail.second][tail.first], visited);
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

	vector<vector<bool>> grid(max_y - min_y + 1);
	for(auto& row : grid)
		row.resize(max_x - min_x + 1);
	grid[-min_y][-min_x] = true;

	uint32_t visited = 1;
	array<point_t, Instruction::length> chain;
	chain.fill({ -min_x, -min_y });

	for(const auto& ins : instructions)
		ins->execute(chain, grid, visited);

	cout << visited << '\n';
	return 0;
}

