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
};

constexpr void dfs_update(Dir* const dir, uint32_t& sum, const uint32_t max_size) {
	for(Dir* const sub_dir : dir->sub_dirs)
		dfs_update(sub_dir, sum, max_size);

	if(dir->parent_dir != nullptr)
		dir->parent_dir->size += dir->size;
	if(dir->size <= max_size)
		sum += dir->size;
}

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
			cur_dir->size += stoi(line);
repeat:
		;
	}

	uint32_t sum = 0;
	dfs_update(&root, sum, max_size);

	cout << sum << '\n';
	return 0;
}
