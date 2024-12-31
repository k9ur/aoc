#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(void)
{
	string s;
	uint32_t safe_count = 0;

	while (getline(cin, s)) {
		istringstream iss(s);
		uint32_t next, prev;
		bool incr, safe = true, first = true;

		iss >> prev;
		while (iss >> next) {
			if ((!first && incr != (next > prev))
				|| abs(static_cast<int64_t>(next) - static_cast<int64_t>(prev)) > 3 || next == prev)
			{
				safe = false;
				break;
			} else if (first) {
				incr = next > prev;
				first = false;
			}
			prev = next;
		}
		if (safe)
			++safe_count;
	}

	cout << safe_count << '\n';
	return 0;
}

