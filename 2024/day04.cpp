#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

using namespace std;

struct WordSearch : vector<string>
{
	constexpr size_t rows(void) const noexcept
	{
		return size();
	}
	constexpr size_t cols(void) const noexcept
	{
		return front().size();
	}
};

bool is_xmas(const Map& ws, const size_t row, const size_t col, const int8_t r_diff, const int8_t c_diff)
{
	const auto a = ws[row + 0 * r_diff][col + 0 * c_diff],
	           b = ws[row + 1 * r_diff][col + 1 * c_diff],
	           c = ws[row + 2 * r_diff][col + 2 * c_diff],
	           d = ws[row + 3 * r_diff][col + 3 * c_diff];
	return (a == 'X' && b == 'M' && c == 'A' && d == 'S')
		|| (d == 'X' && c == 'M' && b == 'A' && a == 'S');
}

int main(void)
{
	WordSearch ws{};
	size_t r, c;
	uint32_t count = 0;

	{
		string s;
		while (getline(cin, s)) {
			ws.push_back(s);
			assert(s.size() == ws.cols());
		}
	}
	if (ws.rows() < 4 || ws.cols() < 4)
		goto done;

	for (r = 0; r != ws.rows(); ++r)
		for (c = 0; c != ws.cols() - 3; ++c)
			if (is_xmas(ws, r, c, 0, +1))
				++count;
	for (c = 0; c != ws.cols(); ++c)
		for (r = 0; r != ws.rows() - 3; ++r)
			if (is_xmas(ws, r, c, +1, 0))
				++count;
	for (r = 0; r != ws.rows() - 3; ++r)
		for (c = 0; c != ws.cols() - 3; ++c)
			if (is_xmas(ws, r, c, +1, +1))
				++count;
	for (r = 0; r != ws.rows() - 3; ++r)
		for (c = 3; c != ws.cols(); ++c)
			if (is_xmas(ws, r, c, +1, -1))
				++count;

done:
	cout << count << '\n';
	return 0;
}

