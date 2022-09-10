#pragma once
#include <string>

#include "Proto/Protocol.hpp"
#include "SendReceive.hpp"
#include <map>
#include <vector>
#include <functional>

/**
 * @brief The client side of the connection.
 *
 */
class ClientEndpoint {
 private:
  typedef std::function<void(std::string)> EventFunc;
  static const int bufferSize = 1024;

 public:
  std::map<wwnet::EMessageType, std::vector<EventFunc>> callbacks;

  int socketFd;
  char buffer[bufferSize] = {0};
  ClientEndpoint();
  void start_connecting(const char* ipv4, int port);
  bool is_connected_and_buffer_empty();
  bool is_up() { return wwnet::is_socket_up(socketFd); }
  void send_data(wwnet::EMessageType msgType, const char* data);
  bool request_map();
  void digest_incoming();
  std::pair<wwnet::EMessageType, std::string> rcv_data();
  void quit();
};
