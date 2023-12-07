#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	vector<uint32_t> needed, count;
	string line;
	size_t c_i = 0;
	string::size_type i, bar_idx = 0;

	while(getline(cin, line)) {
		size_t matches = 0;

		if(!bar_idx)
			bar_idx = line.find_first_of('|');
		else
			assert(line[bar_idx] == '|');
		for(i = 6; line[i] != ':'; ++i);
		i += 2;

		uint32_t num;
		istringstream iss(line.substr(i));
		while(iss >> num)
			needed.push_back(num);

		iss = istringstream(line.substr(bar_idx + 1));
		while(iss >> num)
			if(find(needed.cbegin(), needed.cend(), num) != needed.cend())
				++matches;

		if(c_i >= count.size())
			count.push_back(1);
		for(size_t match_i = 0; match_i != matches; ++match_i) {
			if(c_i + match_i + 1 >= count.size())
				count.push_back(1);
			count[c_i + match_i + 1] += count[c_i];
		}
		needed.clear();
		++c_i;
	}

	cout << accumulate(count.cbegin(), next(count.cbegin(), c_i), 0) << '\n';
	return 0;
}

