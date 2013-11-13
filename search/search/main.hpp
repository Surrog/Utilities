
#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <mutex>
#include <vector>
#include "boost\filesystem.hpp"

typedef std::basic_string<char, std::char_traits<char>, boost::pool_allocator<char>> string_pool;
typedef std::pair<string_pool, int> pair_stringpool_int;

struct Input
{
	boost::filesystem::path root = boost::filesystem::current_path();
	std::string regex;
	bool recursive = false;
	bool content = false;
	bool directoryName = false;
};

struct Output
{
	std::mutex coutLock;
	std::vector< string_pool > fileOpenError;
};

template<typename T, typename Y>
bool find(const T& in, const Y& value)
{
	return in.find(value.c_str()) != T::npos;
}

#endif //!MAIN_HPP