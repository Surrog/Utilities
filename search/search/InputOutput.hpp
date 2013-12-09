
#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <mutex>
#include <vector>
#include <utility>
#include "boost\filesystem.hpp"
#include "boost\pool\pool_alloc.hpp"
#include <ostream>

typedef std::basic_string<char, std::char_traits<char>,
                          boost::pool_allocator<char> > string_pool;
typedef std::pair<string_pool, int> pair_stringpool_int;

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>,
                          boost::pool_allocator<wchar_t> > wstring_pool;
typedef std::pair<wstring_pool, int> pair_wstringpool_int;

typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
                                boost::pool_allocator<wchar_t> > wsstreampool;

struct Input {
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

struct Output {
  std::mutex coutLock;
};

#endif //!INPUT_OUTPUT_HPP
