#ifndef INPUT_HPP
#define INPUT_HPP

#include "afilesystem.hpp"

struct input
{
	bool force_delete;
	bool prompt_before;
	bool prompt_once;
	bool recursive;
	bool verbose;

	astd::filesystem::path input_path;
};

#endif //!INPUT_HPP
