#include <iostream>
#include <list>
#include <iterator>
#include <cstdint>

using namespace std;

struct File
{
	static constexpr size_t gap_ID = 0;

	size_t ID, size;

	constexpr File() {}
	constexpr File(const size_t _ID, const size_t _size)
	  : ID(_ID)
	  , size(_size)
	{}
};

template<input_iterator InputIt>
void advance_both(InputIt& it, size_t& idx, const size_t n)
{
	advance(it, n);
	idx += n;
}

int main(void)
{
	list<File> files; // IDs shifted up by 1 so that 0 = empty
	list<File>::iterator it_last, it_left;
	uint64_t checksum = 0;
	size_t idx_left, idx_last;

	{
		size_t ID = 0;
		char c;
		bool gap = false;
		while (cin >> c) {
			if (!gap)
				++ID;
			if (c != '0')
				files.emplace_back(gap ? File::gap_ID : ID, c - '0');
			gap ^= true;
		}
	}
	if (files.empty())
		goto done;

	it_last  = prev(files.end());
	idx_last = files.size() - 1;
	while (idx_last && it_last->ID == File::gap_ID)
		advance_both(it_last, idx_last, -1);

	while (idx_last) {
		it_left  = files.begin();
		idx_left = 0;
		while (idx_left < idx_last && (it_left->ID != File::gap_ID || it_left->size < it_last->size))
			advance_both(it_left, idx_left, 1);

		if (idx_left < idx_last) {
			if (it_last->size < it_left->size) {
				files.emplace(next(it_left), File::gap_ID, it_left->size - it_last->size);
				++idx_last;
				it_left->size = it_last->size;
			}
			it_left->ID = it_last->ID;
			it_last->ID = File::gap_ID; // don't need to merge gaps since they've already been passed
		}

		do
			advance_both(it_last, idx_last, -1);
		while (idx_last && it_last->ID == File::gap_ID);
	}

	for (size_t idx = 0; const auto& file : files) {
		if (file.ID != File::gap_ID)
			checksum += (file.size * (idx * 2 + file.size - 1)) / 2 * (file.ID - 1);
		idx += file.size;
	}

done:
	cout << checksum << '\n';
	return 0;
}

