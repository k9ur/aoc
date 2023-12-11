#include <iostream>
#include <string>
#include <string_view>
#include <ranges>
#include <array>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	constexpr array<const string_view, 9> words = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	string line;
	string_view line_view;
	string::size_type i, j;
	uint32_t num_sum = 0;

	while(getline(cin, line)) {
		line_view = line;
		for(i = 0; const auto c : line_view) {
			if(isdigit(c)) {
				num_sum += (c - '0') * 10;
				break;
			} else
				for(j = 0; const auto& word : words) {
					++j;
					if(word == line_view.substr(i, word.size())) {
						num_sum += j * 10;
						goto db1; // double break
					}
				}
			++i;
		}
db1:
		for(i = line_view.size() - 1; const auto c : line_view | views::reverse) {
			if(isdigit(c)) {
				num_sum += c - '0';
				break;
			} else
				for(j = 0; const auto& word : words) {
					++j;
					if(word.size() + i > line.size())
						continue;
					else if(word == line_view.substr(i, word.size())) {
						num_sum += j;
						goto db2;
					}
				}
			--i;
		}
db2:
		;
	}

	cout << num_sum << '\n';
	return 0;
}

