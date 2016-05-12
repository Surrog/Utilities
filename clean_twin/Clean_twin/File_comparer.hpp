#ifndef FILE_COMPARER_HPP
#define FILE_COMPARER_HPP

#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include "afilesystem.hpp"
#include "astring_view.hpp"

typedef astd::basic_string_view<astd::filesystem::path::value_type> path_view;
typedef std::basic_ifstream<astd::filesystem::path::value_type> path_fstream;
typedef std::basic_stringstream<astd::filesystem::path::value_type> path_sstream;
typedef std::basic_string<astd::filesystem::path::value_type> file_content_string;

class File_Comparer
{
public:
   enum
   {
      LIMIT_TO_HASH = 10000,
   };

   bool compare_with_hash(const astd::filesystem::path& lv, const astd::filesystem::path& rv)
   {
      return get_hash(lv) == get_hash(rv);
   }

   bool compare_with_content(const astd::filesystem::path& lv, const astd::filesystem::path& rv)
   {
      return get_content(lv) == get_content(rv);
   }

   
   bool operator()(const astd::filesystem::path& lv, const astd::filesystem::path& rv)
   {
      if (is_too_large_for_storing(lv) || is_too_large_for_storing(rv))
      {
         return compare_with_hash(lv, rv);
      }
      return compare_with_content(lv, rv);
   }

private:
   bool is_too_large_for_storing(const astd::filesystem::path& p)
   {
      return std::find_if(file_hash_map.begin(), file_hash_map.end(), 
         [&p](auto& val) {return val.first == p; }) != file_hash_map.end()
         || astd::filesystem::file_size(p) > LIMIT_TO_HASH;
   }

   std::size_t get_hash(const astd::filesystem::path& p)
   {
      auto found = std::find_if(file_hash_map.begin(), file_hash_map.end(), [&](auto& val) {return val.first == p; });
      if (found != file_hash_map.end())
      {
         return found->second;
      }

      auto hash_result = hash_functor(get_content(p));
      file_hash_map.push_back(std::make_pair(p, hash_result));
      sort_hash();
      return hash_result;
   }

   file_content_string get_content(const astd::filesystem::path& file)
   {
      auto found = std::find_if(file_content_map.begin(), file_content_map.end(),
         [&file](auto& val) {return val.first == file; });
      if (found != file_content_map.end())
      {
         return found->second;
      }

      path_fstream stream(file.native(), std::ifstream::binary | std::ifstream::in);
      if (!stream.good())
      {
         std::cout << "error opening " << file << std::endl;
      }

      path_sstream sstr_ref;
      sstr_ref << stream.rdbuf();
      auto string_value = sstr_ref.str();
      if (string_value.size() < LIMIT_TO_HASH)
      {
         file_content_map.push_back(std::make_pair(file, string_value));
         sort_content();
      }
      return string_value;
   }


   void sort_content()
   {
      std::sort(file_content_map.begin(), file_content_map.end(), [](auto& lv, auto& rv) {return lv.first < rv.first; });
   }

   void sort_hash()
   {
      std::sort(file_hash_map.begin(), file_hash_map.end(), [](auto& lv, auto& rv) {return lv.first < rv.first; });
   }

   std::vector< std::pair<astd::filesystem::path, file_content_string> > file_content_map;
   std::vector< std::pair<astd::filesystem::path, std::size_t> > file_hash_map;
   std::hash<file_content_string> hash_functor;
};

#endif //!FILE_COMPARER_HPP