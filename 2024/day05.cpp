#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <execution>
#include <concepts>
#include <cstdint>

using namespace std;

template<totally_ordered T>
bool is_overlap(const set<T>& a, vector<T> b)
{
	vector<T> isect;
	ranges::sort(b);
	set_intersection(execution::par_unseq, a.cbegin(), a.cend(), b.cbegin(), b.cend(), back_inserter(isect));
	return !isect.empty();
}

int main(void)
{
	unordered_map<uint32_t, set<uint32_t>> rules;
	vector<uint32_t> pages;
	string s;
	istringstream iss;
	uint32_t X, Y, page, mid_sum{};
	bool valid;

	while (getline(cin, s)) {
		if (s.empty())
			break;
		iss.str(s);
		iss >> X;
		iss.ignore(1);
		iss >> Y;
		rules[X].insert(Y);

		iss.clear();
	}

	while (getline(cin, s)) {
		valid = true;
		iss.str(s);
		while (iss >> page) {
			if (is_overlap(rules[page], pages)) {
				valid = false;
				break;
			}
			pages.push_back(page);
			iss.ignore(1);
		}
		if (valid)
			mid_sum += pages[pages.size() >> 1];

		iss.clear();
		pages.clear();
	}

	cout << mid_sum << '\n';
	return 0;
}

