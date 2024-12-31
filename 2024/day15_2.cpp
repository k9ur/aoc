#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <ranges>
#include <cassert>
#include <cstdint>

using namespace std;

using Position = pair<size_t, size_t>; // r, c

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

	template<char obstruction_ch, char box_l_ch, char box_r_ch, char empty_ch, int8_t diff_r, int8_t diff_c>
	bool push_box(size_t r, size_t c, vector<Position>& l_boxes) noexcept
	{
		if constexpr (diff_r == 0) {
			c += diff_c;
			char push_into_ch;
			if constexpr (diff_c == 1)
				push_into_ch = (*this)[r][c + 1];
			else
				push_into_ch = (*this)[r][c];

			if (push_into_ch == obstruction_ch)
				return false;
			if (push_into_ch != empty_ch) {
				l_boxes.emplace_back(r, c + diff_c);
				// old box will be overridden, no need to remove
			}

			(*this)[r][c]     = box_l_ch; // replace box
			(*this)[r][c + 1] = box_r_ch; //
		} else {
			r += diff_r;
			const auto push_into_l_ch = (*this)[r][c],
			           push_into_r_ch = (*this)[r][c + 1];

			if (push_into_l_ch == obstruction_ch || push_into_r_ch == obstruction_ch)
				return false;
			if (push_into_l_ch == box_l_ch) {
				l_boxes.emplace_back(r, c);
				(*this)[r][c]     = empty_ch; // remove old box
				(*this)[r][c + 1] = empty_ch; //
			} else {
				if (push_into_l_ch == box_r_ch) {
					l_boxes.emplace_back(r, c - 1);
					(*this)[r][c - 1] = empty_ch; // remove old box
					(*this)[r][c]     = empty_ch; //
				}
				if (push_into_r_ch == box_l_ch) {
					l_boxes.emplace_back(r, c + 1);
					(*this)[r][c + 1] = empty_ch; // remove old box
					(*this)[r][c + 2] = empty_ch; //
				}
			}

			(*this)[r][c]     = box_l_ch; // replace box
			(*this)[r][c + 1] = box_r_ch; //
		}
		return true;
	}

	template<char obstruction_ch, char box_l_ch, char box_r_ch, char robot_ch, char empty_ch, int8_t diff_r, int8_t diff_c>
	void move_robot(size_t& r, size_t& c) noexcept
	{
		static_assert(diff_r <= 1 && diff_r >= -1 && diff_c <= 1 && diff_c >= -1);

		const auto next_ch = (*this)[r + diff_r][c + diff_c];
		if (next_ch == empty_ch) {
			(*this)[r][c] = empty_ch; // move robot
			r += diff_r;
			c += diff_c;
			(*this)[r][c] = robot_ch;
			return;
		} else if (next_ch == obstruction_ch)
			return;
		assert(next_ch == box_l_ch || next_ch == box_r_ch);

		auto this_copy = *this;
		vector<Position> left_boxes, new_left_boxes;

		{
			const auto l_box_c = c + diff_c - (next_ch != box_l_ch);
			left_boxes.emplace_back(r + diff_r, l_box_c);
			this_copy[r + diff_r][l_box_c]     = empty_ch; // remove box
			this_copy[r + diff_r][l_box_c + 1] = empty_ch; //
			this_copy[r][c]                    = empty_ch; // remove robot
			this_copy[r + diff_r][c + diff_c]  = robot_ch; // replace robot
		}
		do {
			new_left_boxes.clear();

			for (const auto& [ l_box_r, l_box_c ] : left_boxes)
				if (!this_copy.push_box<obstruction_ch, box_l_ch, box_r_ch, empty_ch, diff_r, diff_c>(l_box_r, l_box_c, new_left_boxes))
					return;

			left_boxes = new_left_boxes;
		} while (!left_boxes.empty());

		*this = this_copy;
		r += diff_r;
		c += diff_c;
	}
};

int main(void)
{
	constexpr char obstruction = '#',
	               box_left = '[',
	               box_right = ']',
	               box_single = 'O',
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

			string s_actual = "";
			for (const auto c : s)
				switch (c) {
					case robot:
						s_actual.push_back(robot);
						s_actual.push_back(empty);
						break;
					case box_single:
						s_actual.push_back(box_left);
						s_actual.push_back(box_right);
						break;
					case obstruction:
						s_actual.insert(s_actual.end(), 2, obstruction);
						break;
					case empty:
						s_actual.insert(s_actual.end(), 2, empty);
						break;
					default:
						break;
				}

			const auto s_idx = s_actual.find(robot);
			if (s_idx != string::npos) {
				pos_c = s_idx;
				pos_r = map.rows();
			}
			map.push_back(s_actual);
			assert(s_actual.size() == map.cols());
		}
	}
	{
		char c;
		while (cin >> c)
			switch (c) {
				case '^':
					map.move_robot<obstruction, box_left, box_right, robot, empty, -1, 0>(pos_r, pos_c);
					break;
				case 'v':
					map.move_robot<obstruction, box_left, box_right, robot, empty, +1, 0>(pos_r, pos_c);
					break;
				case '>':
					map.move_robot<obstruction, box_left, box_right, robot, empty, 0, +1>(pos_r, pos_c);
					break;
				case '<':
					map.move_robot<obstruction, box_left, box_right, robot, empty, 0, -1>(pos_r, pos_c);
					break;
				case '\n':
				default:
					break;
			}
	}

	for (const auto r : views::iota(size_t{}, map.rows()))
		for (const auto c : views::iota(size_t{}, map.cols()))
			if (map[r][c] == box_left)
				gps_sum += r * 100 + c;

	for (const auto& s : map)
		cout << s << endl;

	cout << gps_sum << '\n';
	return 0;
}

