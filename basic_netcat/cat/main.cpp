
#include <iostream>
#include <fstream>

#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/variant.hpp>
#include <thread>
#include <future>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <sstream>

#include "afilesystem.hpp"

std::string& get_path_type(astd::filesystem::file_status& stat)
{
   static std::unordered_map<astd::filesystem::file_type, std::string> type_array{
       { astd::filesystem::file_type::not_found, "<ERR>"}, 
       { astd::filesystem::file_type::none, "<ERR>"},
       { astd::filesystem::file_type::regular, "     "}, 
       { astd::filesystem::file_type::directory, "<DIR>"},
       { astd::filesystem::file_type::symlink, "<LNK>"}, 
       { astd::filesystem::file_type::block, "<BLK>"},
       { astd::filesystem::file_type::character, "<CHR>"}, 
       { astd::filesystem::file_type::fifo, "<FIF>"},
       { astd::filesystem::file_type::socket, "<SOK>"}, 
       { astd::filesystem::file_type::unknown, "<UNK>"}};

   return type_array[stat.type()];
}

auto dump_file(
    astd::filesystem::path& path, bool line_num, std::istream::streampos pos = std::istream::streampos())
{
   std::ifstream file(path.c_str(), std::ifstream::in | std::ifstream::binary);
   if(file.good() && file.seekg(pos))
   {
      file.sync_with_stdio(false);
      std::cout.sync_with_stdio(false);
	  if (!line_num)
	  {
		  std::cout << file.rdbuf();
	  }
	  else
	  {
		  std::string buffer;
		  static int line = 0;
		  static bool new_line = true;
		  while (std::getline(file, buffer))
		  {
			  if (new_line)
			  {
				  line++;
				  std::cout << line << '\t';
			  }
			  if (buffer.size())
				  std::cout << buffer;
			  new_line = file.good();
			  if (new_line)
				  std::cout << std::endl;
		  }
	  }
	  file.clear();
	  pos = file.tellg();
   }
   else if (!pos) //avoiding pollution of the cmd output if we've already openend once the file
   {
      std::cerr << "error not found : " << path << std::endl;
   }
   return pos;
}

std::stringstream dump_directory_stream(
    astd::filesystem::path path, bool recurs, int cur_width)
{
   std::stringstream result;
   result.sync_with_stdio(false);

   typedef std::future<std::stringstream> future_stringstream;
   std::vector<future_stringstream> directory_result;

   astd::filesystem::directory_iterator it;
   astd::filesystem::directory_iterator ite;
   if(recurs)
   {
      it = astd::filesystem::directory_iterator(path);

      std::for_each(it, ite, [&](const astd::filesystem::path& sub_path)
          {
             auto st = astd::filesystem::status(sub_path);

             if(astd::filesystem::is_directory(st))
             {
                directory_result.push_back(std::async(&dump_directory_stream,
                    std::move(*it), recurs, cur_width + 4)); //-V112
             }
          });
   }

   it = astd::filesystem::directory_iterator(path);
   std::size_t directory_index = 0;
   std::for_each(it, ite, [&](const astd::filesystem::path& sub_path)
       {
          auto st = astd::filesystem::status(sub_path);

          for(int i = 0; i < cur_width; ++i)
             result << " ";
          result << get_path_type(st) << " - " << sub_path << std::endl;
          if(recurs && astd::filesystem::is_directory(st))
          {
             result << std::endl;
             result << directory_result[directory_index].get().rdbuf()
                    << std::endl;
             directory_index++;
          }
       });

   return result;
}

void dump_directory(
    const astd::filesystem::path& path, bool recurs, std::size_t space_width)
{
   astd::filesystem::directory_iterator it(path);
   astd::filesystem::directory_iterator ite;

   std::for_each(it, ite, [&](const astd::filesystem::path& sub_path)
       {
          auto st = astd::filesystem::status(sub_path);
          for(std::size_t i = 0; i < space_width; ++i)
             std::cout << " ";
          std::cout << get_path_type(st) << " - " << sub_path << std::endl;
          if(recurs && astd::filesystem::is_directory(st))
          {
             for(std::size_t i = 0; i < space_width; ++i)
                std::cout << " ";
             std::cout << "--->" << std::endl;
             dump_directory(sub_path, recurs, space_width + 4); //-V112
             std::cout << std::endl;
          }
       });
}

int main(int argc, char** argv)
{
   boost::program_options::options_description desc("Allowed options");
   desc.add_options()("help", "produce help message")
	   ("t", "keep reading the file to display new write")
	   ("r", "will recursively display the content of all directory")
	   ("n", "display the line number while reading the file")
	   ("path", boost::program_options::value<astd::filesystem::path>(), "path to display");

   boost::program_options::positional_options_description p;
   p.add("path", -1);

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
      return 0;
   }

   if(vm.count("help"))
   {
      std::cout << desc << std::endl;
   }

   bool trail = vm.find("t") != vm.end();
   bool recurs = vm.find("r") != vm.end();
   bool line_num = vm.find("n") != vm.end();
   auto it = vm.find("path");

   if(it != vm.end())
   {
      auto path = it->second.as<astd::filesystem::path>();
      auto st = astd::filesystem::status(path);
      if(astd::filesystem::is_directory(st))
      {
         dump_directory(path, recurs, 0);
      }
      if(astd::filesystem::is_regular_file(st))
      {
         if(!trail)
         {
            dump_file(path, line_num);
         }
         else
         {
            auto time = astd::filesystem::last_write_time(path);
            auto pos = dump_file(path, line_num);
            while(true)
            {
               auto neo = astd::filesystem::last_write_time(path);
               if(time != neo)
               {
                  pos = dump_file(path, line_num, pos);
                  time = neo;
               }
               
               std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
         }
      }
   }
   else
   {
      dump_directory(astd::filesystem::current_path(), recurs, 0);
   }

   return 0;
}