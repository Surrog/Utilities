#ifndef SEARCH_TASK_HPP
#define SEARCH_TASK_HPP

#include "InputOutput.hpp"
#include "afilesystem.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <future>
#include <sstream>

struct search_task
{
   private:
   const Input& _input;
   Output _output;

   public:
   search_task(const Input& input);

   void do_search();

   const Output& getOutput();

   string_pool search_file(const astd::filesystem::path& path);

   string_pool match_directory(const astd::filesystem::path& path);

   string_pool search_directory(const astd::filesystem::path& path);

   std::future<string_pool> search(
      astd::filesystem::path path, bool recurcive);

   std::vector<pair_stringpool_int, boost::pool_allocator<pair_stringpool_int> >
   search_content(const astd::filesystem::path& path) const;

   template <typename T, typename Y>
   static bool find(const T& input, const Y& value, int& input_it, int value_it)
   {
      int input_size = input.size();
      int value_size = value.size();

      while (input_it < input_size && value_it < value_size)
      {
         if (input[input_it] == value[value_it])
         {
            input_it++;
            value_it++;
         }
         else if (value[value_it] == '*')
         {
            value_it++;
            if (value_it >= value_size)
               return true;
            while (input_it < input_size && value_it < value_size)
            {
               if (find(input, value, input_it, value_it))
                  return true;
               input_it++;
            }
         }
         else
         {
            return false;
         }
      }
      while (value_it < value_size && value[value_it] == '*')
         value_it++;

      return value_it >= value_size && input_it >= input_size;
   }

   template <typename T, typename Y>
   static bool find(const T& input, const Y& value)
   {
      int input_it = 0;
      int value_it = 0;
      return find(input, value, input_it, value_it);
   }

   bool has_extension(const astd::filesystem::path& path)
   {
      return has_extension(path.native());
   }

   template <typename T, typename Alloc>
   bool has_extension(
       const std::basic_string<T, std::char_traits<T>, Alloc>& char_array)
   {
      for (const T& char_value : char_array)
      {
         if (char_value == static_cast<T>('.'))
            return true;
      }
      return false;
   }
};

#endif //! SEARCH_TASK_HPP
