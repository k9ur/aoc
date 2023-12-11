#include <iostream>
#include <cassert>
#include <string>
#include <cstdint>

using namespace std;

int main(void)
{
	constexpr string::size_type size = 4;
	string line;
	string::size_type ind = 0;
	uint32_t bits;

	cin >> line;
	assert(line.size() >= size);
	char c;

repeat:
	bits = 0;
	for(string::size_type i = 0; i != size; ++i) {
		c = line[i + ind] - 'a';
		if(bits >> c & 1) {
			assert(++ind <= line.size());
			goto repeat;
		} else
			bits |= 1 << c;
	}

	cout << ind + size << '\n';
	return 0;
}

