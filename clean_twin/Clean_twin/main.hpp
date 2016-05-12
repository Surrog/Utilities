#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

#include "afilesystem.hpp"
#include "astring_view.hpp"


enum class error_code : int {
   NONE,
   BAD_INPUT_NO_PATH,
   BAD_INPUT_PATH
};

typedef astd::basic_string_view<astd::filesystem::path::value_type> path_view;
typedef std::basic_ifstream<astd::filesystem::path::value_type> path_fstream;
typedef std::basic_stringstream<astd::filesystem::path::value_type> path_sstream;
typedef std::basic_string<astd::filesystem::path::value_type> file_content_string;

const astd::filesystem::path& choose_the_bad_file(const astd::filesystem::path& lv, const astd::filesystem::path& rv);
std::vector<astd::filesystem::path> delve_into_directories(astd::filesystem::path path);
auto search_files_to_delete(const std::vector<astd::filesystem::path>& file_list);
error_code clean_directory(astd::filesystem::path path);

#endif //!MAIN_HPP
