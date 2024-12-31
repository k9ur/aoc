#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <cassert>
#include <cstdint>

using namespace std;

struct Map : vector<string>
{
	constexpr size_t rows(void) const noexcept
	{
		return size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return front().size();
	}

	template<char obstruction_ch, char box_ch, char robot_ch, char empty_ch, int8_t diff_r, int8_t diff_c>
	void move_robot(size_t& r, size_t& c) noexcept
	{
		static_assert(diff_r <= 1 && diff_r >= -1 && diff_c <= 1 && diff_c >= -1);
		assert((*this)[r][c] == robot_ch);

		size_t next_r = r,
		       next_c = c;
		do {
			next_r += diff_r;
			next_c += diff_c;
		} while ((*this)[next_r][next_c] == box_ch);
		if ((*this)[next_r][next_c] == obstruction_ch)
			return;

		(*this)[next_r][next_c] = box_ch;
		(*this)[r][c] = empty_ch;
		r += diff_r;
		c += diff_c;
		(*this)[r][c] = robot_ch;
	}
};

int main(void)
{
	constexpr char obstruction = '#',
	               box = 'O',
	               robot = '@',
	               empty = '.';

	Map map{};
	uint32_t gps_sum = 0;
	size_t pos_r{}, pos_c{};

	{
		string s;
		while (getline(cin, s)) {
			if (s.empty())
				break;
			const auto s_idx = s.find(robot);
			if (s_idx != string::npos) {
				pos_c = s_idx;
				pos_r = map.rows();
			}
			map.push_back(s);
			assert(s.size() == map.cols());
		}
	}
	{
		char c;
		while (cin >> c)
			switch (c) {
				case '^':
					map.move_robot<obstruction, box, robot, empty, -1, 0>(pos_r, pos_c);
					break;
				case 'v':
					map.move_robot<obstruction, box, robot, empty, +1, 0>(pos_r, pos_c);
					break;
				case '>':
					map.move_robot<obstruction, box, robot, empty, 0, +1>(pos_r, pos_c);
					break;
				case '<':
					map.move_robot<obstruction, box, robot, empty, 0, -1>(pos_r, pos_c);
					break;
				case '\n':
				default:
					break;
			}
	}

	for (const auto r : views::iota(size_t{}, map.rows()))
		for (const auto c : views::iota(size_t{}, map.cols()))
			if (map[r][c] == box)
				gps_sum += r * 100 + c;

	cout << gps_sum << '\n';
	return 0;
}

