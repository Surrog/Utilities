
#include <iostream>
#include <regex>
#include <fstream>
#include <utility>
#include <future>

#include "boost\filesystem.hpp"
#include "boost\pool\pool_alloc.hpp"
#include "main.hpp"

struct search_task
{
private:
	const Input& _input;
	Output _output;

public:

	search_task(const Input& input)
		: _input(input), _output()
	{}
	
	void do_search()
	{
		search(_input.root, true);
	}

	const Output& getOutput()
	{
		return _output;
	}

	string_pool output_path(const boost::filesystem::path& input)
	{
		string_pool result(input.generic_string().c_str());
		for (char& c : result)
		{
			if (c == '/')
				c = '\\';
		}
		return result;
	}

	void search_file(const boost::filesystem::path& path)
	{
		if (find(path.leaf().string(), _input.regex)) //search on name
		{
			std::lock_guard<std::mutex> g(_output.coutLock);
			std::cout << output_path(path).c_str() << std::endl;
		}

		if (_input.content) // search in content
		{
			std::ifstream stream(path.string());
			if (stream.good())
			{
				string_pool buffer;
				buffer.reserve(128);
				std::vector< pair_stringpool_int, boost::pool_allocator< pair_stringpool_int > > result_search;
				int line = 0;
				while (std::getline(stream, buffer))
				{
					if (find(buffer, _input.regex))
						result_search.push_back(std::make_pair(std::move(buffer), line));
					line++;
				}
				if (result_search.size()) // matching line found
				{
					std::lock_guard<std::mutex> g(_output.coutLock);
					std::cout << output_path(path) << "\r\n";
					for (auto pair : result_search)
					{
						std::cout << "line " << pair.second << " : " << pair.first << "\r\n";
					}
					std::cout.flush();
				}
			}
			else // open file error
			{
				_output.fileOpenError.push_back(output_path(path));
			}
		}
	}

	void search_directory(const boost::filesystem::path& path)
	{
		if (_input.directoryName && find(path.leaf().string(), _input.regex))
		{
			std::lock_guard<std::mutex> g(_output.coutLock);
			std::cout << output_path(path) << std::endl;
		}

		std::vector< std::future< void > > directory_finished;
		std::for_each(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(),
			[this, &directory_finished](const boost::filesystem::path& subpath){
			directory_finished.push_back(std::async(&search_task::search, this, subpath, _input.recursive));
		});
		for (auto& future : directory_finished)
		{
			future.wait();
		}
	}

	void search(boost::filesystem::path path, bool recurcive)
	{
		boost::system::error_code error;
		if (recurcive && boost::filesystem::is_directory(path, error) && !error)
		{
			std::async(&search_task::search_directory, this, path).wait();
		}
		else if (boost::filesystem::is_regular_file(path, error) && !error)
		{
			std::async(&search_task::search_file, this, path).wait();
		}
	}
};



int main(int argc, char** argv)
{
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