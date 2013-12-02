
#include <future>
#include <iostream>
#include "search_task.hpp"

search_task::search_task(const Input &input) : _input(input), _output() {}

void search_task::do_search() {
  auto result = search(_input.root, true);
  std::wcout << result.get().str();
  std::wcout.flush();
}

const Output &search_task::getOutput() { return _output; }

std::vector<pair_wstringpool_int, boost::pool_allocator<pair_wstringpool_int> >
search_task::search_content(const boost::filesystem::path &path,
                            const std::string &regex) {
  std::vector<pair_wstringpool_int,
              boost::pool_allocator<pair_wstringpool_int> > result;
  std::ifstream stream(path.c_str());
  if (stream.good()) {

    stream.sync_with_stdio(false); // allow threaded input

    string_pool buffer;
    buffer.reserve(128);
    int line = 0;

    while (std::getline(stream, buffer)) {
      if (find(buffer, regex)) {
        int size = buffer.size();
        result.push_back(
            std::make_pair(wstring_pool(buffer.begin(), buffer.end()), line));
        buffer.reserve(size);
      }
      line++;
    }
  }
  return result;
}

wsstreampool search_task::search_file(const boost::filesystem::path &path) {
  wsstreampool result;
  if (_input.filename &&
      find(path.filename().native(), _input.regex)) // search on name
  {
    output_line(path, result);
  }

  if (_input.filterfilename &&
      !find(path.leaf().string(), _input.filterEx)) // filter on file name
    return result;

  if (_input.content) // search in content
  {
    auto content_result =
        search_content(path, _input.regex); // get every line matching
    if (content_result.size()) {
      output_line(path, result);
      for (auto pair : content_result) {
        result << "line " << pair.second << " : " << pair.first << "\r\n";
      }
    }
  }
  return result;
}

wsstreampool
search_task::search_directory(const boost::filesystem::path &path) {
  wsstreampool result;
  if (_input.directoryName &&
      find(path.leaf().string(), _input.regex)) { // check directory name
    output_line(path, result);
  }

  if (_input.filterdirectoryName && // directory filter
      !find(path.leaf().string(), _input.filterEx))
    return result;

  std::vector<std::future<wsstreampool> > directory_finished; // explore every
                                                              // file in it
  boost::system::error_code error;
  auto begin = boost::filesystem::directory_iterator(path, error);
  auto end = boost::filesystem::directory_iterator();
  if (!error) {
    std::for_each(begin, end, [this, &directory_finished](
                                  const boost::filesystem::path &subpath) {
      directory_finished.push_back(search(std::move(subpath), _input.recursive));
    });

    for (auto &future : directory_finished) {
      result << future.get().str();
    }
  }
  std::unique_lock<std::mutex> g(_output.coutLock, std::defer_lock);
  if (g.try_lock()) {
    std::wcout << result.str();
    std::wcout.flush();
    return wsstreampool();
  }
  return result;
}

std::future<wsstreampool> search_task::search(boost::filesystem::path path,
                                              bool recurcive) {
  boost::system::error_code error;
  auto stat = boost::filesystem::status(path, error);
  if (!error) {
    auto type = stat.type();
    if (recurcive && type == boost::filesystem::file_type::directory_file) {
      return std::async(
          thread_task(&search_task::search_directory, this, std::move(path)));
    } else if (type == boost::filesystem::file_type::regular_file) {
      return std::async(
          thread_task(&search_task::search_file, this, std::move(path)));
    }
  }
  return std::async([]() { return wsstreampool(); });
}
