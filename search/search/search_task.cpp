
#include <future>
#include <iostream>
#include <fstream>
#include "search_task.hpp"
#include "afilesystem.hpp"
#include <functional>

search_task::search_task(const Input& input)
    : _input(input)
    , _output()
{
}

void search_task::do_search()
{
   auto result = search(_input.root, true);
   output(result.get());
}

const Output& search_task::getOutput() { return _output; }

std::vector<pair_stringpool_int, boost::pool_allocator<pair_stringpool_int> >
search_task::search_content(const astd::filesystem::path& path) const
{
   std::vector<pair_stringpool_int, boost::pool_allocator<pair_stringpool_int> >
       result;
   std::ifstream stream(path.c_str());
   bool filter_found = !_input.filterfilecontent;
   if (stream.good())
   {

      stream.sync_with_stdio(false); // allow threaded input

      plain_string_pool buffer;
      buffer.reserve(128);
      int line = 0;

      static auto filterEx_pattern = setup_pattern(_input.filterEx);
      static auto regex_pattern = setup_pattern(_input.regex);

      while (std::getline(stream, buffer))
      {
         if (!filter_found && find_Ex(buffer, filterEx_pattern))
         {
            filter_found = true;
         }
         if (find_Ex(buffer, regex_pattern))
         {
            auto size = buffer.size();
            result.push_back(std::make_pair(
                string_pool(buffer.begin(), buffer.end()), line));
            buffer.reserve(size);
         }
         line++;
      }
   }
   if (!filter_found)
      return std::vector<pair_stringpool_int,
          boost::pool_allocator<pair_stringpool_int> >();
   return result;
}

string_pool search_task::search_file(const astd::filesystem::path& path)
{
   string_pool result;
   if (_input.filterdirectoryName
       && !find(path.parent_path().native(), _input.filterEx))
      return result;

   static auto filterEx_pattern = setup_pattern(_input.filterEx);
   static auto regex_pattern = setup_pattern(_input.regex);

   if (_input.filename
       && find_Ex(filename(path), regex_pattern)) // search on name
   {
      output_line(path.c_str(), result);
   }

   if (_input.filterfilename
       && !find_Ex(filename(path), filterEx_pattern)) // filter on file name
      return result;

   if (_input.content) // search in content
   {
      auto content_result = search_content(path); // get every line matching
      if (content_result.size())
      {
         output_line("\r\n==\r\n", result);
         output_line(path.c_str(), result);
         output_line("==\r\n", result);

         for (auto pair : content_result)
         {
            output("line ", result);
            output(pair.second, result);
            output(" : ", result);
            output_line(pair.first, result);

            // result << "line " << pair.second << " : " << pair.first <<
            // "\r\n";
         }
      }
   }
   return result;
}

string_pool search_task::match_directory(const astd::filesystem::path& path)
{
   string_pool result;

   static auto filterEx_pattern = setup_pattern(_input.filterEx);
   static auto regex_pattern = setup_pattern(_input.regex);

   if (_input.filterdirectoryName
       && !find_Ex(path.native(), filterEx_pattern)) // filter by path
      return result;

   if (_input.directoryName && find_Ex(filename(path), regex_pattern))
   { // check directory name
      output_line(path.c_str(), result);
   }
   return result;
}

string_pool search_task::search_directory(const astd::filesystem::path& path)
{
   string_pool result = match_directory(path);

   std::vector<std::future<string_pool> > directory_finished; // explore every
   // file in it
   astd::filesystem::error_code error;
   auto begin = astd::filesystem::directory_iterator(path, error);
   auto end = astd::filesystem::directory_iterator();
   if (!error)
   {
      std::for_each(begin, end,
          [this, &directory_finished](const astd::filesystem::path& subpath)
          {
             directory_finished.push_back(
                 search(std::move(subpath), _input.recursive));
          });

      for (auto& future : directory_finished)
      {
         output(future.get(), result);
      }
   }
   std::unique_lock<std::mutex> g(_output.coutLock, std::defer_lock);
   if (g.try_lock())
   {
      output(result);
      return string_pool();
   }
   return result;
}

std::future<string_pool> search_task::search(
   astd::filesystem::path path, bool recurcive)
{
   auto st = astd::filesystem::status(path);

   if (recurcive && astd::filesystem::is_directory(st))
   {
      return std::async(std::launch::deferred,
          std::bind(&search_task::search_directory, this, std::move(path)));
   }
   else if (astd::filesystem::is_regular_file(st))
   {
      return std::async(std::launch::deferred,
          std::bind(&search_task::search_file, this, std::move(path)));
   }

   return std::async([]()
       {
          return string_pool();
       });
}
