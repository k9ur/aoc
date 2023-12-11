#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

int main(void)
{
	string line;
	uint32_t score = 0;

	while(getline(cin, line)) {
		if(line == "A X")
			score += 3 + 0;
		else if(line == "A Y")
			score += 1 + 3;
		else if(line == "A Z")
			score += 2 + 6;
		else if(line == "B X")
			score += 1 + 0;
		else if(line == "B Y")
			score += 2 + 3;
		else if(line == "B Z")
			score += 3 + 6;
		else if(line == "C X")
			score += 2 + 0;
		else if(line == "C Y")
			score += 3 + 3;
		else if(line == "C Z")
			score += 1 + 6;
		else
			exit(EXIT_FAILURE);
	}

	cout << score << '\n';
	return 0;
}

