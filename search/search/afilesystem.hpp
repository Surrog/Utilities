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
   typedef std::experimental::filesystem::file_status file_status;
   typedef std::experimental::filesystem::file_type file_type;
   typedef std::error_code error_code;
   using std::experimental::filesystem::current_path;
   using std::experimental::filesystem::exists;
   using std::experimental::filesystem::remove;
   using std::experimental::filesystem::is_regular_file;
   using std::experimental::filesystem::is_directory;
   using std::experimental::filesystem::status;
   using std::experimental::filesystem::last_write_time;
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
   typedef boost::filesystem::file_status file_status;
   struct file_type
   {
      enum Type {
         not_found = boost::filesystem::file_not_found,
         none = boost::filesystem::status_unknown,
         regular = boost::filesystem::regular_file,
         directory = boost::filesystem::directory_file,
         symlink = boost::filesystem::symlink_file,
         block = boost::filesystem::block_file,
         character = boost::filesystem::character_file,
         fifo = boost::filesystem::fifo_file,
         socket = boost::filesystem::socket_file,
         unknown = boost::filesystem::type_unknown
      };
   private:
      Type _value;
   public:

      file_type(Type value)
         : _value(value)
      {}

      file_type(boost::filesystem::file_type value)
         : _value(static_cast<Type>(value))
      {}

      operator Type()
      {
         return _value;
      }
      operator boost::filesystem::file_type()
      {
         return static_cast<boost::filesystem::file_type>(_value);
      }

      Type get() const { return _value; }

      bool operator<(const file_type& rvalue) const
      {
         return _value < rvalue._value;
      }

      template <typename T>
      bool operator<(const T&& rvalue) const
      {
         return _value < rvalue._value;
      }

      template <typename T>
      bool operator>(const T&& rvalue) const
      {
         return !operator<(rvalue);
      }

      bool operator==(const file_type& rvalue) const
      {
         return _value == rvalue._value;
      }

      template <typename T>
      bool operator==(const T&& rvalue) const
      {
         return _value == rvalue._value;
      }
   };
   
   typedef boost::system::error_code error_code;
   using boost::filesystem::current_path;
   using boost::filesystem::exists;
   using boost::filesystem::remove;
   using boost::filesystem::is_regular_file;
   using boost::filesystem::is_directory;
   using boost::filesystem::status;
   using boost::filesystem::last_write_time;
}
}

#endif

#endif //! AFILESYSTEM_HPP
