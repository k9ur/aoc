#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

constexpr uint32_t get_score(const vector<string>& trees, const size_t r, const size_t c, const size_t rows, const size_t cols) {
	const char val = trees[r][c];
	size_t y, x;

	for(y = r - 1; y && trees[y][c] < val; --y);
	uint32_t score = r - y;
	for(y = r + 1; y < rows - 1 && trees[y][c] < val; ++y);
	score *= y - r;
	for(x = c - 1; x && trees[r][x] < val; --x);
	score *= c - x;
	for(x = c + 1; x < cols - 1 && trees[r][x] < val; ++x);
	score *= x - c;

	return score;
}

int main(void) {
	vector<string> trees;
	string line;
	string::size_type size = 0;

	while(getline(cin, line)) {
		trees.push_back(line);
		if(size) [[likely]]
			assert(size == line.size());
		else [[unlikely]]
			assert(size = line.size());
	}

	const size_t cols = size,
	             rows = trees.size();
	uint32_t max_score = 0;

	for(size_t r = 1; r < rows - 1; ++r) // Ignore outer trees since they'll have a score of 0
		for(size_t c = 1; c < cols - 1; ++c) {
			const uint32_t score = get_score(trees, r, c, rows, cols);
			if(score > max_score)
				max_score = score;
		}

	cout << max_score << '\n';
	return 0;
}
