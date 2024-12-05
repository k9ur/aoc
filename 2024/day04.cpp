#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

bool is_xmas(const char a, const char b, const char c, const char d)
{
	return (a == 'X' && b == 'M' && c == 'A' && d == 'S')
		|| (d == 'X' && c == 'M' && b == 'A' && a == 'S');
}

int main(void)
{
	vector<string> ws
	string s;
	size_t rows, cols;
	uint32_t r, c, count{};

	while (getline(cin, s))
		ws.push_back(s);
	rows = ws.size();
	cols = ws.front().size();
	if (rows < 4 || cols < 4)
		return 0;

	for (const auto& row : ws)
		for (c = 0; c != cols - 3; ++c)
			if (is_xmas(row[c], row[c + 1], row[c + 2], row[c + 3]))
				++count;
	for (c = 0; c != cols; ++c)
		for (r = 0; r != rows - 3; ++r)
			if (is_xmas(ws[r][c], ws[r + 1][c], ws[r + 2][c], ws[r + 3][c]))
				++count;
	for (r = 0; r != rows - 3; ++r)
		for (c = 0; c != cols - 3; ++c)
			if (is_xmas(ws[r][c], ws[r + 1][c + 1], ws[r + 2][c + 2], ws[r + 3][c + 3]))
				++count;
	for (r = 0; r != rows - 3; ++r)
		for (c = 3; c != cols; ++c)
			if (is_xmas(ws[r][c], ws[r + 1][c - 1], ws[r + 2][c - 2], ws[r + 3][c - 3]))
				++count;

	cout << count << '\n';
	return 0;
}

