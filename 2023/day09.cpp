#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<int32_t> seq;
	string line;
	int32_t num, num_sum = 0;

	while(getline(cin, line)) {
		istringstream iss(line);
		while(iss >> num)
			seq.push_back(num);

		while(ranges::any_of(seq, identity())) {
			for(size_t i = 0; i < seq.size() - 1; ++i)
				seq[i] = seq[i + 1] - seq[i];
			num_sum += seq.back();
			seq.erase(seq.end() - 1);
		}

		seq.clear();
	}

	cout << num_sum << '\n';
	return 0;
}

