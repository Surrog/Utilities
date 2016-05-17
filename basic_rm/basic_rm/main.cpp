
#include <cctype>
#include <iostream>

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include "afilesystem.hpp"
#include "input.hpp"
#include "main.hpp"

std::pair<bool, input> parse_input(
    const boost::program_options::variables_map& vm)
{
   std::pair<bool, input> result;

   result.second.force_delete = vm.find("f") != vm.end();
   result.second.prompt_before = vm.find("i") != vm.end();
   result.second.prompt_once = vm.find("I") != vm.end();
   result.second.recursive = vm.find("r") != vm.end();
   result.second.verbose = vm.find("v") != vm.end();

   auto path = vm.find("path");
   if(path != vm.end())
   {
      auto path_string = path->second.as<std::string>();
      result.second.input_path
          = astd::filesystem::path(path_string);
      result.first = astd::filesystem::exists(result.second.input_path);
      if(!result.first)
         std::cerr << result.second.input_path << " doesn't exist" << std::endl;
   }
   else
   {
      result.first = false;
   }
   return result;
}

bool delete_directory(
    const input& inp, const astd::filesystem::path& path_to_delete)
{
   astd::filesystem::directory_iterator it
       = astd::filesystem::directory_iterator(path_to_delete);
   astd::filesystem::directory_iterator ite;

   bool confirm_delete = true;

   if(inp.recursive)
   {
      while(it != ite)
      {
         confirm_delete = delete_function(inp, *it) && confirm_delete;
         ++it;
      }
   }

   if(it == ite && confirm_delete)
   {
      std::error_code code;
      bool ok_remove = astd::filesystem::remove(path_to_delete, code);
      if (!ok_remove)
      {
         std::cerr << "failed to delete directory " << path_to_delete << std::endl;
         std::cerr << code.message() << std::endl;
      }
      return ok_remove;
   }
   else
   {
      std::cerr << "cannot delete non empty directory. if you used the "
                   "recursive option, then a file have not been correctly "
                   "deleted"
                << std::endl;
   }

   return false;
}

bool delete_file(const input& inp) { return delete_file(inp, inp.input_path); }

bool ask_for_delete(const astd::filesystem::path& path_to_delete)
{
   char ans = 0;
   std::cout << "would you like to delete " << path_to_delete << " ? (Y/N)"
             << std::endl;
   while(ans != 'Y' && ans != 'N')
   {
      std::cin >> ans;
      ans = std::toupper(ans);
   }

   return ans == 'Y';
}

bool delete_file(const input& inp, const astd::filesystem::path& path_to_delete)
{
   static bool delete_confirm = true;
   bool result = false;
   static bool prompted = false;
   if(!inp.force_delete
       && (inp.prompt_before || (inp.prompt_once && !prompted)))
   {
      prompted = true;
      delete_confirm = ask_for_delete(path_to_delete);
   }

   if(delete_confirm)
   {
      if(!inp.force_delete && inp.verbose)
      {
         std::cout << "deleting " << path_to_delete << std::endl;
      }

      std::error_code code;
      result = astd::filesystem::remove(path_to_delete, code);
      if (!result) {
         std::cerr << "delete of "<< path_to_delete << " failed" << std::endl;
         std::cerr << code.message() << std::endl;
      }
   }
   return result;
}

bool delete_function(
    const input& inp, const astd::filesystem::path& path_to_delete)
{
   bool result;
   if(astd::filesystem::is_directory(path_to_delete))
   {
      result = delete_directory(inp, path_to_delete);
   }
   else
   {
      result = delete_file(inp, path_to_delete);
   }
   return result;
}

bool delete_function(const input& inp)
{
   return delete_function(inp, inp.input_path);
}

int main(int argc, char** argv)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")(
       "f", "ignore nonexistent files, never prompt")(
       "i", "prompt before every removal")("I",
       "prompt only once. Less intrusive than - i, while still giv"
       "ing protection against most mistakes")(
       "r", "remove directories and their contents recursively")(
       "v", "explain what is being done")("path",
       boost::program_options::value<std::string>(),
       "path to element being remove");

   boost::program_options::positional_options_description p;
   p.add("path", -1);

   int result = 0;

   boost::program_options::variables_map vm;
   try
   {
      boost::program_options::store(
          boost::program_options::command_line_parser(argc, argv)
              .options(desc)
              .positional(p)
              .style(boost::program_options::command_line_style::unix_style
                  | boost::program_options::command_line_style::
                        allow_long_disguise)
              .allow_unregistered()
              .run(),
          vm);
      boost::program_options::notify(vm);
   }
   catch(boost::program_options::error e)
   {
      std::cout << desc << std::endl;
      result = 0;
   }

   if(vm.count("help"))
   {
      std::cout << desc << std::endl;
   }
   else
   {
      auto _input = parse_input(vm);
      if(_input.first)
      {
         if(!_input.second.force_delete)
            std::cerr << "starting deletion of " << _input.second.input_path
                      << std::endl;
         if(!delete_function(_input.second))
         {
            result = 2;
         }
      }
      else
      {
         if(!_input.second.force_delete)
         {
            std::cerr << "invalid input" << std::endl;
            std::cerr << desc << std::endl;
         }

         result = 1;
      }
   }

   return result;
}