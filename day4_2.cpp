#include <iostream>
#include <cassert>
#include <stdint.h>

using namespace std;

int main(void) {
	uint32_t a_lower, a_upper, b_lower, b_upper, count = 0;

	while(!cin.eof()) {
		cin >> a_lower;
		cin.get();
		cin >> a_upper;
		cin.get();
		cin >> b_lower;
		cin.get();
		cin >> b_upper;

		assert(a_upper >= a_lower && b_upper >= b_lower);
		if(a_upper >= b_lower && b_upper >= a_lower)
			++count;
	}

	cout << count << '\n';
	return 0;
}
