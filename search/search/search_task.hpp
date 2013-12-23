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

  struct thread_task {
    typedef wsstreampool (search_task::*func_ptr)(
        const boost::filesystem::path &);

    func_ptr _function;
    search_task *_instance;
    boost::filesystem::path _path;

    thread_task(func_ptr function, search_task *instance,
                boost::filesystem::path path)
        : _function(function), _instance(instance), _path(path) {}

    wsstreampool operator()() { return (_instance->*_function)(_path); }
  };

public:
  search_task(const Input &input);

  void do_search();

  const Output &getOutput();

  wsstreampool search_file(const boost::filesystem::path &path);

  wsstreampool match_directory(const boost::filesystem::path &path);

  wsstreampool search_directory(const boost::filesystem::path &path);

  std::future<wsstreampool> search(boost::filesystem::path path,
                                   bool recurcive);

  std::vector<pair_wstringpool_int,
              boost::pool_allocator<pair_wstringpool_int> >
  search_content(const boost::filesystem::path &path) const;

  template <typename T, typename Y>
  static bool find(const T &input, const Y &value, int &input_it,
                   int value_it) {
    int input_size = input.size();
    int value_size = value.size();

    while (input_it < input_size && value_it < value_size) {
      if (input[input_it] == value[value_it]) {
        input_it++;
        value_it++;
      } else if (value[value_it] == '*') {
        value_it++;
        if (value_it >= value_size)
          return true;
        while (input_it < input_size && value_it < value_size) {
          if (find(input, value, input_it, value_it))
            return true;
          input_it++;
        }
      } else {
        return false;
      }
    }
    while (value_it < value_size && value[value_it] == '*')
      value_it++;

    return value_it >= value_size && input_it >= input_size;
  }

  template <typename T, typename Y>
  static bool find(const T &input, const Y &value) {
    int input_it = 0;
    int value_it = 0;
    return find(input, value, input_it, value_it);
  }

  template <typename T>
  void output_line(const T &value, std::ostream &stream = std::cout) {
    stream << value << "\r\n";
  }

  template <typename T>
  void output_line(
      const std::basic_string<wchar_t, std::char_traits<wchar_t>, T> &value,
      std::wostream &stream = std::wcout) {
    stream << value << "\r\n";
  }

  template <typename T>
  void output_line(const boost::filesystem::path &input,
                   std::basic_ostream<T> &stream) {
    output_line(input.native(), stream);
  }

  bool has_extension(const boost::filesystem::path &path) {
    return has_extension(path.native());
  }

  template <typename T, typename Alloc>
  bool has_extension(
      const std::basic_string<T, std::char_traits<T>, Alloc> &char_array) {
    for (const T &char_value : char_array) {
      if (char_value == static_cast<T>('.'))
        return true;
    }
    return false;
  }

  template <typename T> int stat(const T *path, _stat &buf) {
    return _stat(path, &buf);
  }

  int stat(const wchar_t *path, _stat &buf) { return _wstat(path, &buf); }
};

#endif //!SEARCH_TASK_HPP
