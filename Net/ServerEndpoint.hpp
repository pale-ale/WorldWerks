#pragma once

#include <netinet/in.h>
#include <vector>
#include <string>

/**
 * @brief Save some information about a player, i.e. his connection and username.
 * 
 */
struct PlayerConnection{
  std::string playerName = "Unnamed";
  int clientFd = -1;
};

/**
 * @brief The server side of the connection.
 * 
 */
class ServerEndpoint {
  public:
  std::vector<PlayerConnection> connections; 
  struct sockaddr_in address;
  ServerEndpoint(const char *ipv4, int port);
  PlayerConnection accept_connection();
  void send_single(int clientFd, const char* data);
  std::string rcv_single(int clientFd);
  void send_all(const char* data);

  private:
  char buffer[1024] = {0};
  int serverFd;
};
