#ifndef SEARCH_TASK_HPP
#define SEARCH_TASK_HPP

#include "boost\filesystem.hpp"
#include "InputOutput.hpp"

#include <future>
#include <sstream>

struct search_task {
private:
  const Input &_input;
  Output _output;

public:
  search_task(const Input &input);

  void do_search();

  const Output &getOutput();

  wsstreampool search_file(const boost::filesystem::path &path);

  wsstreampool search_directory(const boost::filesystem::path &path);

  std::future< wsstreampool > search(boost::filesystem::path path, bool recurcive);
  std::future< wsstreampool > search(boost::filesystem::path path);

  std::vector<pair_wstringpool_int, boost::pool_allocator<pair_wstringpool_int> >
  static search_content(const boost::filesystem::path &path,
                        const std::string & regex);

  template <typename T, typename Y>
  static bool find(const T &input, const Y &value) {
    int input_it = 0;
    int value_it = 0;
    int input_size = input.size();
    int value_size = value.size();

    while (input_it < input_size && value_it < value_size) {
      if (input[input_it] == value[value_it]) {
        input_it++;
        value_it++;
      } else if (value[value_it] == '*') {
        while (value[value_it] == '*')
          value_it++;
        if (value_it >= value_size)
          return true;
        while (input_it < input_size && value_it < value_size &&
               input[input_it] != value[value_it]) {
          input_it++;
        }
      } else {
        return false;
      }
    }
    while (input_it >= input_size && value_it < value_size &&
           value[value_it] == '*') {
      value_it++;
    }

    return value_it >= value_size && input_it >= input_size;
  }

  template< typename T >
  void output_line(const T& value, std::ostream& stream = std::cout)
  {
	  stream << value << "\r\n";
  }

  void output_line(const wchar_t* const& value, std::wostream& stream = std::wcout)
  {
	  stream << value << "\r\n";
  }

  template <typename T>
  void output_line(const std::basic_string<wchar_t, std::char_traits<wchar_t>, T>& value, std::wostream& stream = std::wcout)
  {
	  stream << value << "\r\n";
  }

  template <typename T>
  void output_line(const boost::filesystem::path &input, std::basic_ostream< T >& stream) {
	  output_line(input.c_str(), stream);
  }
};

#endif //!SEARCH_TASK_HPP
