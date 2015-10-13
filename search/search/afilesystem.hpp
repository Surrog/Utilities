#ifndef AFILESYSTEM_HPP
#define AFILESYSTEM_HPP

#if _MSC_VER > 1700
#include <experimental/filesystem>

namespace astd
{
namespace filesystem
{
   typedef std::experimental::filesystem::path path;
   typedef std::experimental::filesystem::directory_iterator directory_iterator;
   using std::experimental::filesystem::exists;
   using std::experimental::filesystem::remove;
   using std::experimental::filesystem::is_regular_file;
   using std::experimental::filesystem::is_directory;
}
}

#else

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

namespace astd
{
namespace filesystem
{
   typedef boost::filesystem::path path;
   typedef boost::filesystem::directory_iterator directory_iterator;
   using boost::filesystem::exists;
   using boost::filesystem::remove;
   using boost::filesystem::is_regular_file;
   using boost::filesystem::is_directory;
}
}

#endif

#endif //! AFILESYSTEM_HPP
