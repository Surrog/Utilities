
#include <future>
#include <iostream>
#include "search_task.hpp"

search_task::search_task(const Input& input)
: _input(input), _output()
{}

void search_task::do_search()
{
	search(_input.root, true);
}

const Output& search_task::getOutput()
{
	return _output;
}

string_pool search_task::output_path(const boost::filesystem::path& input)
{
	string_pool result(input.generic_string().c_str());
	for (char& c : result)
	{
		if (c == '/')
			c = '\\';
	}
	return result;
}

void search_task::search_file(const boost::filesystem::path& path)
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

void search_task::search_directory(const boost::filesystem::path& path)
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

void search_task::search(boost::filesystem::path path, bool recurcive)
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

