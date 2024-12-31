#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(void)
{
	constexpr size_t gap_ID = 0;

	vector<size_t> blocks; // IDs shifted up by 1 so that 0 = empty
	uint64_t checksum = 0;
	size_t idx_l{}, idx_r;

	{
		size_t ID = 0;
		char c;
		bool gap = false;
		while (cin >> c) {
			blocks.insert(blocks.end(), c - '0', gap ? gap_ID : ++ID);
			gap ^= true;
		}
	}
	if (blocks.empty())
		goto done;

	idx_r = blocks.size() - 1;
	while (idx_l < idx_r && blocks[idx_l] != gap_ID)
		++idx_l;
	while (idx_l < idx_r && blocks[idx_r] == gap_ID)
		--idx_r;

	while (idx_l < idx_r) {
		blocks[idx_l] = blocks[idx_r];
		do
			++idx_l;
		while (idx_l < idx_r && blocks[idx_l] != gap_ID);
		do
			--idx_r;
		while (blocks[idx_r] == gap_ID);
	}

	for (idx_l = 0; const auto block : blocks) {
		if (idx_l > idx_r)
			break;
		checksum += idx_l * (block - 1);
		++idx_l;
	}

done:
	cout << checksum << '\n';
	return 0;
}

