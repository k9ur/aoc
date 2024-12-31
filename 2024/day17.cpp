#include <iostream>
#include <vector>
#include <array>
#include <span>
#include <iterator>
#include <cassert>
#include <cstdint>

using namespace std;

template<bool combo = false>
uint32_t get_operand(const array<uint32_t, 3>& regs, const char operand)
{
	if constexpr (combo)
		switch (operand) {
			case '0':
			case '1':
			case '2':
			case '3':
				return operand - '0';
			case '4':
				return regs[0];
			case '5':
				return regs[1];
			case '6':
				return regs[2];
		}
	else
		return operand - '0';

	assert(false);
	return 0;
}

void perform_op(array<uint32_t, 3>& regs, vector<char>& output, size_t& ip, const char opcode, const char operand)
{
	ip += 2;
	switch (opcode) {
		case '0':
			regs[0] >>= get_operand<true>(regs, operand);
			break;
		case '1':
			regs[1] ^= get_operand<false>(regs, operand);
			break;
		case '2':
			regs[1] = get_operand<true>(regs, operand) % 8;
			break;
		case '3':
			if (regs[0])
				ip = get_operand<false>(regs, operand) * 2;
			break;
		case '4':
			regs[1] ^= regs[2];
			break;
		case '5':
			output.push_back(get_operand<true>(regs, operand) % 8 + '0');
			break;
		case '6':
			regs[1] = regs[0] >> get_operand<true>(regs, operand);
			break;
		case '7':
			regs[2] = regs[0] >> get_operand<true>(regs, operand);
			break;
		default:
			break;
	}
}

int main(void)
{
	array<uint32_t, 3> regs;
	vector<char> program, output;
	size_t ip = 0;

	cin.ignore(12);
	cin >> regs[0];
	cin.ignore(13);
	cin >> regs[1];
	cin.ignore(13);
	cin >> regs[2];
	cin.ignore(11);

	{
		char c;
		while (cin >> c) {
			program.push_back(c);
			if (cin)
				cin.ignore(1);
		}
	}

	while (ip < program.size())
		perform_op(regs, output, ip, program[ip], program[ip + 1]);

	for (const auto c : span(output.cbegin(), prev(output.cend())))
		cout << c << ',';
	cout << output.back() << '\n';

	return 0;
}

