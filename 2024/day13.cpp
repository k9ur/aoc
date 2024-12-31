#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cassert>
#include <cstdint>

using namespace std;

template<uint8_t max_presses>
void solve_best(double& num_main, double& num_other, const double frac_start, const double frac_change)
{
	num_main = frac_start;
	num_other = 0.0;
	do {
		if (num_main <= max_presses && num_main == floor(num_main))
			return;
		++num_other;
		num_main -= frac_change;
		if (num_main < 0.0 || num_other > max_presses)
			break;
	} while (true);
	num_main = 0.0;
	num_other = 0.0;
}

template<uint8_t max_presses, uint8_t a_tokens, uint8_t b_tokens>
void solve(const double a1, const double b1, const double c1, const double a2, const double b2, const double c2, double& out_a, double& out_b)
{
	const double det = a1 * b2 - b1 * a2;
	if (det) {
		out_a = (c1 * b2 - b1 * c2) / det;
		out_b = (a1 * c2 - c1 * a2) / det;
		if (out_a > max_presses || out_b > max_presses) {
			out_a = 0.0;
			out_b = 0.0;
		}
	} else if (a1 * c2 == c1 * a2) { // infinitely many solutions
		if (a1 * b_tokens <= b1 * a_tokens) // want maximal b
			solve_best<max_presses>(out_b, out_a, c1 / b1, a1 / b1);
		else // want maximal a
			solve_best<max_presses>(out_a, out_b, c1 / a1, b1 / a1);
	} // else no solutions
}

int main(void)
{
	constexpr uint8_t max_presses = 100,
	                  a_tokens = 3,
	                  b_tokens = 1;

	uint32_t tokens = 0;

	while (cin) {
		string s;
		istringstream iss;
		uint32_t X_A, Y_A, X_B, Y_B, X_prize, Y_prize;

		getline(cin, s);
		iss.str(s);
		iss.ignore(12);
		iss >> X_A;
		iss.ignore(4);
		iss >> Y_A;
		iss.clear();
		getline(cin, s);
		iss.str(s);
		iss.ignore(12);
		iss >> X_B;
		iss.ignore(4);
		iss >> Y_B;
		iss.clear();
		getline(cin, s);
		iss.str(s);
		iss.ignore(9);
		iss >> X_prize;
		iss.ignore(4);
		iss >> Y_prize;
		if (cin) {
			getline(cin, s);
			assert(s.empty());
		}

		double a_presses{}, b_presses{};
		solve<max_presses, a_tokens, b_tokens>(X_A, X_B, X_prize, Y_A, Y_B, Y_prize, a_presses, b_presses);

		if (a_presses == floor(a_presses) && b_presses == floor(b_presses))
			tokens += a_presses * a_tokens + b_presses * b_tokens;
	}

	cout << tokens << '\n';
	return 0;
}

