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

bool is_xmas(const char a, const char b, const char c, const char d)
{
	return (a == 'M' && b == 'M' && c == 'S' && d == 'S')
		|| (a == 'S' && b == 'M' && c == 'S' && d == 'M')
		|| (a == 'S' && b == 'S' && c == 'M' && d == 'M')
		|| (a == 'M' && b == 'S' && c == 'M' && d == 'S');
}

int main(void)
{
	WordSearch ws{};
	uint32_t count = 0;

	{
		string s;
		while (getline(cin, s)) {
			ws.push_back(s);
			assert(s.size() == ws.front().size());
		}
	}
	if (ws.rows() < 3 || ws.cols() < 3)
		goto done;

	for (size_t c, r = 1; r != ws.rows() - 1; ++r)
		for (c = 1; c != ws.cols() - 1; ++c)
			if (ws[r][c] == 'A' && is_xmas(ws[r - 1][c - 1], ws[r - 1][c + 1], ws[r + 1][c - 1], ws[r + 1][c + 1]))
				++count;

done:
	cout << count << '\n';
	return 0;
}

