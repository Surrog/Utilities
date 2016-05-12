
#include <iostream>
#include <thread>
#include <array>

#include <sdkddkver.h> //Win_init define

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

void display_usage()
{
   std::cerr << "xnc "
                "-l to listen"
                "[destination][port]"
             << std::endl;
}

boost::asio::ip::tcp::socket connect_socket(boost::asio::io_service& io_service,
    char* host, char* port, boost::system::error_code& ec)
{
   boost::asio::ip::tcp::resolver resolver(io_service);
   auto endpoint_iterator = resolver.resolve({host, port});
   boost::asio::ip::tcp::socket socket(io_service);
   boost::asio::connect(socket, endpoint_iterator, ec);
   return socket;
}

boost::asio::ip::tcp::socket listen_socket(boost::asio::io_service& io_service,
    char* port, boost::system::error_code& ec)
{
   boost::asio::ip::tcp::acceptor acceptor(io_service);
   boost::asio::ip::tcp::socket socket(io_service);
   acceptor.accept(socket, ec);
   return socket;
}

void run_socket_io(boost::asio::ip::tcp::socket& socket)
{
   std::array<char, 1024> read_buffer;
   std::array<char, 1024> write_buffer;

   socket.async_read_some(boost::asio::buffer(write_buffer, 1024),
       [&write_buffer](const boost::system::error_code& ec, std::size_t length)
       {
          if (!ec)
             std::cout.write(write_buffer.data(), length);
       });

   while (std::cin.getline(read_buffer.data(), 1024))
   {
      socket.send(
          boost::asio::buffer(read_buffer, std::strlen(read_buffer.data())));
   }
}

int main(int argc, char** argv)
{
   if (argc < 3)
   {
      display_usage();
      return 1;
   }

   boost::asio::io_service io_service;
   std::thread t([&io_service]()
       {
          io_service.run();
       });
   boost::system::error_code ec;
   if (std::string(argv[1]) != "-l")
   {
      boost::asio::ip::tcp::socket socket
          = connect_socket(io_service, argv[1], argv[2], ec);
      if (!ec)
         run_socket_io(socket);
   }
   else
   {
      boost::asio::ip::tcp::socket socket
          = listen_socket(io_service, argv[2], ec);
      if (!ec)
         run_socket_io(socket);
   }

   if (ec)
   {
      std::cout << ec.message() << std::endl;
      io_service.stop();
      t.join();
      return 0;
   }

   return 0;
}