#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<uint64_t> vals;
	string line;

	getline(cin, line);
	uint64_t og_val;
	istringstream iss(line.substr(7));
	while(iss >> og_val)
		vals.push_back(og_val);

	vector<bool> done_yet(vals.size(), false);

	getline(cin, line); // Skip empty line
	while(!cin.eof()) {
		getline(cin, line); // Skip map name
		getline(cin, line);
		do {
			uint64_t dest, src, len;
			iss = istringstream(line);
			iss >> dest >> src >> len;

			for(size_t idx = 0; auto& val : vals) {
				if(!done_yet[idx] && val >= src && val < src + len) {
					val -= src;
					val += dest;
					done_yet[idx] = true;
				}
				++idx;
			}
			getline(cin, line);
		} while(!cin.eof() && !line.empty());

		ranges::fill(done_yet, false);
	}

	const auto min_loc = ranges::min_element(vals);
	assert(min_loc != vals.cend());
	cout << *min_loc << '\n';
	return 0;
}

