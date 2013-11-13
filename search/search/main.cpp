
#include <iostream>

#include "InputOutput.hpp"
#include "search_task.hpp"


int main(int argc, char** argv)
{
	std::cout.sync_with_stdio(false);
	Input input;
	std::string regexValue;
	int result = 0;

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg[0] == '-')
		{
			for (char c : arg)
			{
				if (c == 'r')
					input.recursive = true;
				if (c == 'c')
					input.content = true;
				if (c == 'd')
					input.directoryName = true;
				if (c == 't')
				{
					i++;
					if (i < argc)
					{
						input.root = argv[i];
					}
					else
					{
						std::cout << "error : no target chosen" << std::endl;
						result = 1;
					}
				}
			}
		}
		else
		{
			regexValue = std::move(arg);
		}
	}

	if (!result && !regexValue.size())
	{
		std::cout << "error: no search value" << std::endl;
		result = 2;
	}
	input.regex = regexValue;

	if (!result)
	{
		std::cout << "search " << input.regex << " in " << input.root << std::endl;
		std::cout << "recursive " << input.recursive << std::endl;
		std::cout << "include directory name " << input.directoryName << std::endl;
		std::cout << "search in file content " << input.content << std::endl;
		search_task task(input);
		task.do_search();
		for (auto error : task.getOutput().fileOpenError)
		{
			std::cout << "cannot open file " << error << std::endl;
		}
	}
	else
	{
		std::cout << "search [-crd] [-t root path] regex\r\n-c search in file content\r\n-r recurse on sub directory\r\n-d search in directory name\t\n-t follow by a path, set the search location";
	}
	return result;
}