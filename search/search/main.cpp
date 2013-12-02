
#include <iostream>

#include "InputOutput.hpp"
#include "search_task.hpp"

int main(int argc, char **argv) {
  std::cout.sync_with_stdio(false);
  Input input;
  std::string regexValue;
  int result = 0;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] == '-') {
      if (arg[1] == 'f') {
        if (arg[2] == 'n')
          input.filterfilename = true;
        if (arg[2] == 'd')
          input.filterdirectoryName = true;

        i++;
        if (i < argc) {
          input.filterEx = argv[i];
        } else {
          std::cout << "error : filter not found";
          result = 1;
        }
      } else {
        for (char c : arg) {
          if (c == 'r')
            input.recursive = true;
          if (c == 'c')
            input.content = true;
          if (c == 'd')
            input.directoryName = true;
          if (c == 'n')
            input.filename = true;
          if (c == 't') {
            i++;
            if (i < argc) {
              input.root = argv[i];
            } else {
              std::cout << "error : no target chosen" << std::endl;
              result = 2;
            }
          }
        }
      }

    } else {
      regexValue = std::move(arg);
    }
  }

  if (!input.filename && !input.directoryName && !input.content) {
    input.filename = true;
  }

  if (!result && !regexValue.size()) {
    std::cout << "error: no search value" << std::endl;
    result = 3;
  }
  input.regex = regexValue;

  if (!result) {
    std::cout << "search \"" << input.regex << "\"" << std::endl;
    std::cout << "target " << input.root << std::endl;
    std::cout << "recursive " << input.recursive << std::endl;
    std::cout << "filename " << input.filename << std::endl;
    std::cout << "directory name " << input.directoryName << std::endl;
    std::cout << "file content " << input.content << std::endl;
    if (input.filterfilename || input.filterdirectoryName ||
        input.filterfilecontent)
      std::cout << "filter \"" << input.filterEx << "\"" << std::endl;
    std::cout << "filter on filename " << input.filterfilename << std::endl;
    std::cout << "filter on directory name " << input.filterdirectoryName
              << std::endl;
    /*std::cout << "filter on file content " << input.filterfilecontent <<
     * std::endl;*/
    std::cout << std::endl;
    search_task task(input);
    task.do_search();
  } else {
    std::cout << "search [-rcnd] [-t root_path] [-f[nd] filter_regex] regex\r\n"
                 "-n search in filename\r\n"
                 "-c search in file content\r\n"
                 "-d search in directory name\t\n"
                 "-r recurse on sub directory\r\n"
                 "-t followed by a path, set the search location\r\n"
                 "-f [nd] followed by a pattern: will cut short the recursion "
                 "or content search";
    std::cout.flush();
  }
  std::cin.get();
  return result;
}
