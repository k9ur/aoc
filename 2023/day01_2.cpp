#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <cstdint>
#include <cctype>

using namespace std;

int main(void)
{
	constexpr array<const string_view, 9> words = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	uint32_t num_sum = 0;
	string line;
	string_view line_view;
	string::size_type i;

	while(getline(cin, line)) {
		line_view = line;
		for(i = 0; i != line_view.size(); ++i) {
			if(isdigit(line_view[i])) {
				num_sum += (line_view[i] - '0') * 10;
				break;
			} else
				for(size_t j = 0; j != words.size(); ++j)
					if(words[j] == line_view.substr(i, words[j].size())) {
						num_sum += (j + 1) * 10;
						goto db1; // double break
					}
		}
db1:
		for(i = line_view.size(); i; --i) {
			if(isdigit(line_view[i - 1])) {
				num_sum += line_view[i - 1] - '0';
				break;
			} else
				for(size_t j = 0; j != words.size(); ++j) {
					if(words[j].size() + i - 1 > line.size())
						continue;
					else if(words[j] == line_view.substr(i - 1, words[j].size())) {
						num_sum += j + 1;
						goto db2;
					}
				}
		}
db2:
		;
	}

	cout << num_sum << '\n';
	return 0;
}

