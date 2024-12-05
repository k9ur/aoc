#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(void)
{
	string s;
	istringstream iss;
	uint32_t next, prev, safe_count{};
	bool incr, safe, first;

	while (getline(cin, s)) {
		safe = first = true;
		iss.str(s);
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
		iss.clear();
	}

	cout << safe_count << '\n';
	return 0;
}

