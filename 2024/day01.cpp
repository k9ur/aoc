#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<uint32_t> l, r;
	uint32_t dist = 0;

	{
		uint32_t i_l, i_r;
		while (cin >> i_l >> i_r) {
			l.push_back(i_l);
			r.push_back(i_r);
		}
	}

	ranges::sort(l);
	ranges::sort(r);

	for (const auto i : views::iota(size_t{}, l.size()))
		dist += abs(static_cast<int64_t>(l[i]) - static_cast<int64_t>(r[i]));

	cout << dist << '\n';
	return 0;
}

