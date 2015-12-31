#ifndef SEARCH_TASK_HPP
#define SEARCH_TASK_HPP

#include "InputOutput.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/utility/string_ref.hpp"
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
   static auto start_with(T string, const Y& pattern)
   {
      auto pattern_size = pattern.size();
      auto result = true;
      auto begin = string.begin();
      
      if (pattern_size != 0)
         result = pattern_equal(begin, begin + pattern_size, pattern.begin(), pattern.end());
      return std::make_pair(result, pattern_size);
   }

   template <typename T, typename Y>
   static auto end_with(T string, const Y& pattern)
   {
      auto pattern_size = pattern.size();
      auto begin = string.begin() + (string.size() - pattern_size);

      if (pattern_size == 0)
         return std::make_pair(true, string.size() - pattern_size);
      return std::make_pair(pattern_equal(begin, string.end(), pattern.begin(), pattern.end()), string.size() - pattern_size);
   }

   template <typename T, typename Y>
   static bool pattern_equal(T lv_begin, T lv_end, Y pattern_begin, Y pattern_end)
   {
      while (lv_begin != lv_end && pattern_begin != pattern_end && *lv_begin == *pattern_begin)
      {
         ++lv_begin;
         ++pattern_begin;
      }
      return pattern_begin == pattern_end;
   }

   template <typename T, typename Y>
   static bool contain(const T& string, std::size_t begin, std::size_t end, Y patterns_begin, Y patterns_end)
   {
      while (begin != end && patterns_begin != patterns_end)
      {
         if (!patterns_begin->size() || pattern_equal(string.begin() + begin, string.begin() + end, patterns_begin->begin(), patterns_begin->end()))
         {
            begin += patterns_begin->size();
            ++patterns_begin;
         }
         else
         {
            ++begin;
         }
      }
      return patterns_begin == patterns_end;
   }



   template <typename T, typename Y>
   static bool find_Ex(const T& input, const Y& pattern)
   {
      static std::vector<Y> split_vec = setup_pattern(pattern);
      return find_Ex(input, split_vec);
   }

   template <typename Y>
   static std::vector<Y> setup_pattern(const Y& pattern) 
   { 
      std::vector<Y> split_vec;
      boost::split(split_vec, pattern, [](typename Y::value_type c) { return c == Y::value_type('*'); });
      return split_vec;
   }

   template <typename T, typename Y>
   static bool find_Ex(const T& input, const std::vector<Y>& split_vec)
   {
      if (split_vec.size() == 1) // no wildcard
      {
         return std::size(input) == std::size(split_vec[0])
             && std::equal(input.begin(), input.end(), split_vec[0].begin());
      }
      std::size_t tot = 0;
      for (auto& str : split_vec)
      {
         tot += std::size(str);
      }
      if (tot >= std::size(input))
         return false;

      auto start_validation = start_with(input, split_vec.front());
      auto end_validation = end_with(input, split_vec.back());
      return start_validation.first && end_validation.first
          && contain(input, start_validation.second, end_validation.second,
                 split_vec.begin() + 1,
                 split_vec.begin() + (split_vec.size() - 1));
   }

   template <typename T, typename Y>
   static bool find(const T& input, const Y& value, std::size_t& input_it, std::size_t value_it)
   {
      auto input_size = input.size();
      auto value_size = value.size();

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
   static bool find(const T& input, const Y& pattern)
   {
      std::size_t input_it = 0;
      std::size_t pattern_it = 0;
      return find(input, pattern, input_it, pattern_it);
      
   }

   static boost::basic_string_ref<astd::filesystem::path::value_type> filename(const astd::filesystem::path& path)
   {
      auto& native = path.native();
      auto is_slash = [](astd::filesystem::path::value_type c)
      { return c == astd::filesystem::path::value_type('/') || c == astd::filesystem::path::value_type('\\'); }; //TODO: add a Guns N' Roses joke in a comment here
      auto rbegin = native.rbegin();

      if (is_slash(native.back())) //this is a directory path
         rbegin++;

      auto last_slash = std::find_if(rbegin, native.rend(), is_slash);

      if (native.rbegin() != native.rend())
      {
         last_slash--; //remove the slash from the name
         return{ &*last_slash, std::size_t(last_slash - rbegin) + 1 };
      }         
      else
         return { native };
   }

   static bool has_extension(const astd::filesystem::path& path)
   {
      return has_extension(filename(path));
   }

   static bool has_extension(
      const boost::basic_string_ref<astd::filesystem::path::value_type>& char_array)
   {

      for (const astd::filesystem::path::value_type& char_value : char_array)
      {
         if (char_value == static_cast<astd::filesystem::path::value_type>('.'))
            return true;
      }
      return false;
   }
};

#endif //! SEARCH_TASK_HPP
