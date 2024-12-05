#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

bool is_xmas(const char a, const char b, const char c, const char d)
{
	return (a == 'M' && b == 'M' && c == 'S' && d == 'S')
		|| (a == 'S' && b == 'M' && c == 'S' && d == 'M')
		|| (a == 'S' && b == 'S' && c == 'M' && d == 'M')
		|| (a == 'M' && b == 'S' && c == 'M' && d == 'S');
}

int main(void)
{
	vector<string> ws;
	string s;
	size_t rows, cols;
	uint32_t r, c, count{};

	while (getline(cin, s))
		ws.push_back(s);
	rows = ws.size();
	cols = ws.front().size();
	if (rows < 3 || cols < 3)
		return 0;

	for (r = 1; r != rows - 1; ++r)
		for (c = 1; c != cols - 1; ++c)
			if (ws[r][c] == 'A' && is_xmas(ws[r - 1][c - 1], ws[r - 1][c + 1], ws[r + 1][c - 1], ws[r + 1][c + 1]))
				++count;

	cout << count << '\n';
	return 0;
}

