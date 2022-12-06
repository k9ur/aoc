#include <iostream>
#include <cassert>
#include <string>
#include <stdint.h>

using namespace std;

int main(void) {
	constexpr string::size_type size = 14;
	string::size_type ind = 0;
	uint32_t bits;
	string line;

	getline(cin, line);
	assert(line.size() >= size);
#if 0
	do {
		bits = 0;
		for(string::size_type i = 0; i < size; ++i)
			bits |= 1 << (line[i + ind] - 'a');
		assert(++ind <= line.size());
	} while(__builtin_popcount(bits) != 4);
	--ind;
#else
	bool continuer = 1;
	char c;

repeat:
	bits = 0;
	for(string::size_type i = 0; i < size; ++i) {
		c = line[i + ind] - 'a';
		if(bits >> c & 1) {
			assert(++ind <= line.size());
			goto repeat;
		} else
			bits |= 1 << c;
	}
#endif

	cout << ind + size << '\n';
	return 0;
}
