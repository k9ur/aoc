#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

int main(void)
{
	uint32_t mul_sum = 0;
	uint16_t X, Y;
	uint8_t count, st{} /* stage */, ins{} /* instruction */;
	char c;
	bool act = true; // activated

	while (cin >> c) {
		if (c == 'm') {
			if (act) {
				st = 1;
				ins = 1;
				X = Y = count = 0;
			} else
				st = ins = 0;
		} else if (c == 'd') {
			st = 1;
			ins = 2;
		} else if (ins == 1) {
			if (count && ((st == 4 && c == ',') || (st == 5 && c == ')'))) {
				count = 0;
				if (++st == 6) {
					st = ins = 0;
					mul_sum += X * Y;
				}
			} else if ((st == 1 && c != 'u') || (st == 2 && c != 'l') || (st == 3 && c != '(') || ((st == 4 || st == 5) && (c < '0' || c > '9' || count == 3 || (count == 0 && c == '0'))))
				st = ins = 0;
			else if (st == 4) {
				X *= 10;
				X += c - '0';
				++count;
			} else if (st == 5) {
				Y *= 10;
				Y += c - '0';
				++count;
			} else
				++st;
		} else if (ins == 2) {
			if ((st == 1 && c != 'o') || (st == 2 && c != '(' && c != 'n'))
				st = ins = 0;
			else if (st == 2 && c == '(')
				ins = 3;
			else if (st == 2 && c == 'n')
				ins = 4;
			else
				++st;
		} else if (ins == 3) {
			if (c == ')')
				act = true;
			st = ins = 0;
		} else if (ins == 4) {
			if ((st == 2 && c != '\'') || (st == 3 && c != 't') || (st == 4 && c != '('))
				st = ins = 0;
			else if (st == 5) {
				if(c == ')')
					act = false;
				st = ins = 0;
			} else
				++st;
		}
	}

	cout << mul_sum << '\n';
	return 0;
}

