#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <execution>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<uint32_t> needed, count;
	string line;
	size_t c_i = 0;

	while(getline(cin, line)) {
		size_t matches = 0;
		uint32_t num;
		istringstream iss(line);
		iss.ignore(iss.str().size(), ':');
		while(iss >> num)
			needed.push_back(num);

		iss = istringstream(line);
		iss.ignore(iss.str().size(), '|');
		while(iss >> num)
			if(ranges::find(needed, num) != needed.cend())
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

	cout << reduce(execution::par_unseq, next(count.cbegin()), next(count.cbegin(), c_i), count.front()) << '\n';
	return 0;
}

