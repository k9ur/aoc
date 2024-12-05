#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>

using namespace std;

bool is_safe(istringstream& iss, uint32_t prev, bool used_c)
{
	uint32_t next;
	bool incr, first = true;

	while (iss >> next) {
		if ((!first && incr != (next > prev))
			|| abs(static_cast<int64_t>(next) - static_cast<int64_t>(prev)) > 3 || next == prev)
		{
			if (used_c)
				return false;
			else {
				used_c = true;
				continue;
			}
		} else if (first) {
			incr = next > prev;
			first = false;
		}
		prev = next;
	}

	return true;
}

int main(void)
{
	string s;
	istringstream iss;
	uint32_t prev, trash, safe_count{};

	while (getline(cin, s)) {
		iss.str(s);
		iss >> prev;
		if (is_safe(iss, prev, false)) {
			++safe_count;
			goto clear;
		}
		iss.clear();
		iss.str(s);
		iss >> trash >> prev;
		if (is_safe(iss, prev, true)) {
			++safe_count;
			goto clear;
		}
		iss.clear();
		iss.str(s);
		iss >> prev >> trash;
		if (is_safe(iss, prev, true))
			++safe_count;
clear:
		iss.clear();
	}
	cout << safe_count << '\n';
	return 0;
}

