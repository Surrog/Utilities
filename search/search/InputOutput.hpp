
#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <mutex>
#include <vector>
#include <utility>
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/pool/pool_alloc.hpp"
#include <ostream>

#ifdef _MSC_VER
typedef std::wstring string_std;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>,
    boost::pool_allocator<wchar_t> > string_pool;
typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
    boost::pool_allocator<wchar_t> > sstreampool;
#else
typedef std::string string_std;
typedef std::basic_string<char, std::char_traits<char>,
    boost::pool_allocator<char> > string_pool;
typedef std::basic_stringstream<char, std::char_traits<char>,
    boost::pool_allocator<char> > sstreampool;
#endif

typedef std::basic_string<char, std::char_traits<char>,
   boost::pool_allocator<char> > plain_string_pool;

typedef std::pair<string_pool, int> pair_stringpool_int;

struct Input
{
   boost::filesystem::path root = boost::filesystem::current_path();

   std::wstring regex;
   bool recursive = false;
   bool content = false;
   bool filename = false;
   bool directoryName = false;

   std::wstring filterEx;
   bool filterfilename = false;
   bool filterfilecontent = false;
   bool filterdirectoryName = false;
};

struct Output
{
   std::mutex coutLock;
};

#endif //! INPUT_OUTPUT_HPP
