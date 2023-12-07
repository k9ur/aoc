#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdint>

using namespace std;

int main(void)
{
	vector<uint32_t> times;
	uint64_t ways = 1;
	uint32_t num;
	string line;

	getline(cin, line);
	istringstream iss(line.substr(10));
	while(iss >> num)
		times.push_back(num);

	getline(cin, line);
	iss = istringstream(line.substr(10));
	size_t i = 0;
	while(iss >> num) {
		++num;
		const uint64_t b = times[i];
		if(b * b < num * 4) {
			ways = 0;
			break;
		}
		const double sqrt_val = sqrt(b * b - num * 4);
		const uint64_t lower = ceil((-sqrt_val - b) / 2),
		               upper = floor((sqrt_val - b) / 2);
		if(upper < lower) {
			ways = 0;
			break;
		}
		ways *= upper - lower + 1;
		++i;
	}

	cout << ways << '\n';
	return 0;
}

