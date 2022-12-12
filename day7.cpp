#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

struct Dir {
	vector<Dir*> sub_dirs;
	Dir* const parent_dir = nullptr;
	const string name;
	uint32_t size = 0;

	Dir(Dir* const _parent_dir, const string&& _name) : parent_dir(_parent_dir), name(move(_name)) {}

	template<uint32_t max_size>
	constexpr void dfs_update(uint32_t& sum) noexcept {
		for(Dir* const sub_dir : sub_dirs)
			sub_dir->dfs_update<max_size>(sum);

		if(parent_dir != nullptr)
			parent_dir->size += size;
		if(size <= max_size)
			sum += size;
	}
};

int main(void) {
	constexpr uint32_t max_size = 100'000;
	string line;
	Dir root = Dir(nullptr, "/");
	Dir* cur_dir = nullptr;

	while(getline(cin, line)) {
		if(line[0] == '$') { // Command

			if(line[2] == 'c') {
				if(line[5] == '.') { // Go back
					cur_dir = cur_dir->parent_dir;
					assert(cur_dir != nullptr);
				} else if(line[5] == '/') // Root
					cur_dir = &root;
				else {
					const string new_dir_name = line.substr(5);
					for(Dir* const sub_dir : cur_dir->sub_dirs)
						if(sub_dir->name == new_dir_name) {
							cur_dir = sub_dir;
							goto repeat;
						}
					exit(EXIT_FAILURE);
				}
			}

		} else if(line[0] == 'd') // New dir
			cur_dir->sub_dirs.push_back(new Dir(cur_dir, line.substr(4)));
		else // File size
			cur_dir->size += stoul(line);
repeat:
		;
	}

	uint32_t sum = 0;
	root.dfs_update<max_size>(sum);

	cout << sum << '\n';
	return 0;
}
