
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <set>

#include "afilesystem.hpp"
#include "astring_view.hpp"
#include "File_comparer.hpp"
#include "main.hpp"


const astd::filesystem::path& choose_the_bad_file(const astd::filesystem::path& lv, const astd::filesystem::path& rv)
{
   path_view rv_view(rv.c_str());
   path_view lv_view(lv.c_str());
   if ((rv_view.size() == lv_view.size() && rv_view > lv_view) 
      ||  rv_view.size() > lv_view.size())
      return rv;
   return lv;
}


std::vector<astd::filesystem::path> delve_into_directories(astd::filesystem::path path)
{
   std::vector<astd::filesystem::path> file_list;
   astd::filesystem::directory_iterator it(path);
   astd::filesystem::directory_iterator ite;

   std::for_each(it, ite, [&](const astd::filesystem::path& path) mutable {
      auto st = astd::filesystem::status(path);
      if (st.type() == astd::filesystem::file_type::directory)
      {
         clean_directory(path);
      }
      else
      {
         file_list.push_back(path);
      }
   });

   return file_list;
}

template<typename ITERATOR, typename FUNCTOR>
auto commutative_search(ITERATOR begin, ITERATOR end, FUNCTOR func)
{
   std::vector<std::pair<ITERATOR::value_type, ITERATOR::value_type>> result;

   while (begin != end && begin + 1 != end)
   {
      std::for_each(begin + 1, end, [&](auto& lv)
      {
         if (func(*begin, lv)) {
            result.emplace_back(std::make_pair(*begin, lv));
         }
      });

      ++begin;
   }

   return result;
}

auto search_files_to_delete(const std::vector<astd::filesystem::path>& file_list)
{
   std::set<astd::filesystem::path> file_to_delete;

   auto result = commutative_search(file_list.begin(), file_list.end(), File_Comparer());

   for (auto& pair : result)
   {
      file_to_delete.insert(choose_the_bad_file(pair.first, pair.second));
   }
   
   return file_to_delete;
}

template <typename Container>
void delete_files(const Container& files_to_delete)
{
   std::cout << "bad files" << std::endl;
   for (auto& bad_file : files_to_delete)
   {
      std::cout << bad_file << std::endl;
      astd::filesystem::remove(bad_file);
   }
}

error_code clean_directory(astd::filesystem::path path)
{
   error_code result = error_code::NONE;
   std::vector<astd::filesystem::path> files_list = delve_into_directories(path);
   auto files_to_delete = search_files_to_delete(files_list);
   delete_files(files_to_delete);
   
   return result;
}

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      std::cout << "clean_music <path>" << std::endl;
      return static_cast<int>(error_code::BAD_INPUT_NO_PATH);
   }

   if (!astd::filesystem::exists(argv[1]) && !astd::filesystem::is_directory(argv[1]))
   {
      std::cout << "path is not valid" << std::endl;
      return static_cast<int>(error_code::BAD_INPUT_PATH);
   }

   return static_cast<int>(clean_directory(argv[1]));
}

