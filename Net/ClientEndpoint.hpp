#pragma once
#include <string>

/**
 * @brief The client side of the connection.
 * 
 */
class ClientEndpoint {
 private:
  static const int bufferSize = 1024;

 public:
  int socketFd;
  char buffer[bufferSize] = {0};
  ClientEndpoint();
  void start_connecting(const char* ipv4, int port);
  bool is_connected_and_buffer_empty();
  bool is_socket_up();
  void send_data(const char* data);
  std::string rcv_data();
  void quit();
};
