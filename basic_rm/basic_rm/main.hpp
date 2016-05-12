#ifndef MAIN_HPP
#define MAIN_HPP

#include <utility>

#include <boost/program_options/variables_map.hpp>

#include "afilesystem.hpp"
#include "input.hpp"


std::pair<bool, input> parse_input(
   const boost::program_options::variables_map& vm);
bool delete_directory(
   const input& inp, const astd::filesystem::path& path_to_delete);
bool delete_file(const input& inp);
bool ask_for_delete(const astd::filesystem::path& path_to_delete);
bool delete_file(
   const input& inp, const astd::filesystem::path& path_to_delete);
bool delete_function(
   const input& inp, const astd::filesystem::path& path_to_delete);
bool delete_function(const input& inp);

#endif //!MAIN_HPP