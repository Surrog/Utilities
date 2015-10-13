#ifndef AFILESYSTEM_HPP
#define AFILESYSTEM_HPP

#if _MSC_VER > 1700
#include <experimental/filesystem>
#include <system_error>

namespace astd
{
namespace filesystem
{
   typedef std::experimental::filesystem::path path;
   typedef std::experimental::filesystem::directory_iterator directory_iterator;
   typedef std::error_code error_code;
   using std::experimental::filesystem::current_path;
   using std::experimental::filesystem::exists;
   using std::experimental::filesystem::remove;
   using std::experimental::filesystem::is_regular_file;
   using std::experimental::filesystem::is_directory;
   using std::experimental::filesystem::status;
}
}

#else

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/system/error_code.hpp"

namespace astd
{
namespace filesystem
{
   typedef boost::filesystem::path path;
   typedef boost::filesystem::directory_iterator directory_iterator;
   typedef boost::system::error_code error_code;
   using boost::filesystem::current_path;
   using boost::filesystem::exists;
   using boost::filesystem::remove;
   using boost::filesystem::is_regular_file;
   using boost::filesystem::is_directory;
   using boost::filesystem::status;
}
}

#endif

#endif //! AFILESYSTEM_HPP
