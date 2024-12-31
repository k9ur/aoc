#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <ranges>
#include <cassert>
#include <cstdint>

using namespace std;

template<bool combo = false>
uint32_t get_operand(const array<uint64_t, 3>& regs, const char operand)
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

bool perform_op(array<uint64_t, 3>& regs, string& output, size_t& ip, const char opcode, const char operand)
{
	bool out = false;
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
			out = true;
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
	return out;
}

uint64_t simplified_op(const string& program)
{
	vector<uint64_t> new_As, As = { 0 };

	for (const auto c : program | views::reverse) {
		new_As.clear();

		for (auto& A : As) {
			uint8_t Amod8;
			A <<= 3;
			for (Amod8 = 0; Amod8 != 8; ++Amod8) {
				const char next_output = (Amod8 ^ (A >> (Amod8 ^ 3)) ^ 6) % 8 + '0';
				if (next_output == c)
					new_As.push_back(A);
				++A;
			}
		}

		As = new_As;
		assert(!As.empty());
	}

	return As.front(); // first will be the smallest
}

int main(void)
{
	const string input = "2413750315415530"s;

	array<uint64_t, 3> regs;
	string program;

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

	uint64_t A = 0;
	if (program == input && regs[1] == 0 && regs[2] == 0)
		A = simplified_op(program);
	else { // very slow bruteforce
		string output;
again:
		size_t ip = 0;
		bool valid = true;
		output.clear();

		regs[0] = ++A;
		while (ip < program.size())
			if (perform_op(regs, output, ip, program[ip], program[ip + 1]) && output.back() != program[output.size() - 1]) {
				valid = false;
				break;
			}
		if (!valid || output.size() != program.size())
			goto again;
	}

	cout << A << '\n';
	return 0;
}

