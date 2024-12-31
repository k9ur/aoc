#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<uint32_t> l, r;
	uint32_t similarity = 0;

	{
		uint32_t i_l, i_r;
		while (cin >> i_l >> i_r) {
			l.push_back(i_l);
			r.push_back(i_r);
		}
	}

	for (const auto i : l)
		similarity += i * ranges::count(r, i);

	cout << similarity << '\n';
	return 0;
}

