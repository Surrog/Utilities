
#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <mutex>
#include <vector>
#include <utility>
#include "boost\filesystem.hpp"
#include "boost\pool\pool_alloc.hpp"

typedef std::basic_string<char, std::char_traits<char>,
                          boost::pool_allocator<char> > string_pool;
typedef std::pair<string_pool, int> pair_stringpool_int;

struct Input {
  boost::filesystem::path root = boost::filesystem::current_path();

  std::string regex;
  bool recursive = false;
  bool content = false;
  bool filename = false;
  bool directoryName = false;

  std::string filterEx;
  bool filterfilename = false;
  bool filterfilecontent = false;
  bool filterdirectoryName = false;
};

struct Output {
  std::mutex coutLock;
};

#endif //!INPUT_OUTPUT_HPP
