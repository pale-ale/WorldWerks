#include "ServerEndpoint.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "SendReceive.hpp"

ServerEndpoint::ServerEndpoint(const char *ipv4, int port) {
  serverFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (serverFd == -1) {
    printf("[Server]: Could not create a new socket. %d\n", errno);
    exit(1);
  }
  int option_value = 1;
  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option_value,
                 sizeof(option_value)) == -1) {
    printf("[Server]: Could not setsockopt().\n");
    exit(1);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = port;

  if (bind(serverFd, (sockaddr *)&address, sizeof(address))) {
    printf("[Server]: Could not bind.\n");
    exit(1);
  }

  if (listen(serverFd, 10) == -1) {
    printf("[Server]: Cannot listen: %d\n", errno);
    exit(1);
  }
}

/**
 * @brief Try to accept a single incoming connection. If no such connection exists,
 * clientFd will be -1.
 *
 * @return PlayerConnection ---  The accepted connection
 */
PlayerConnection ServerEndpoint::accept_connection() {
  PlayerConnection pc;
  int addressLength = sizeof(address);
  pc.clientFd = accept(serverFd, (sockaddr *)&address, (socklen_t *)&addressLength);
  if (pc.clientFd == -1) {
    if (errno == 11) {  // No connection to accept exists yet.
      return pc;
    }
    printf("[Server]: Could not accept a socket: %d\n", errno);
    exit(1);
  }
  int flags = fcntl(pc.clientFd, F_GETFL);
  if (flags == -1) {
    printf("[Server]: Could not get flags of accepted socket: %d\n", errno);
    exit(1);
  }
  flags |= O_NONBLOCK;
  if (fcntl(pc.clientFd, F_SETFL, flags) != 0) {
    printf("[Server]: Could not set accepted connection to non-blocking mode: %d\n",
           errno);
    exit(1);
  }
  printf("[Server]: Connection accepted.\n");
  // close(pc.clientFd);
  // shutdown(serverFd, SHUT_RDWR);
  return pc;
}

/**
 * @brief Send data to a single client.
 *
 * @param clientFd The client to send to
 * @param msgType The type of the message
 * @param data The data to send
 */
void ServerEndpoint::send_single(int clientFd, wwnet::EMessageType msgType,
                                 const char *data) {
  wwnet::send_data(clientFd, msgType, data, "Server");
}

/**
 * @brief Send data to every connected client.
 *
 * @param msgType The type of the message
 * @param data The data to send
 */
void ServerEndpoint::send_all(wwnet::EMessageType msgType, const char *data) {
  for (auto &&player : connections) {
    send_single(player.clientFd, msgType, data);
  }
}

/**
 * @brief Receive data from a single connected client.
 *
 * @param clientFd The client to read from
 * @return std::pair<wwnet::EMessageType, std::string> --- The message type and its
 * contents
 */
std::pair<wwnet::EMessageType, std::string> ServerEndpoint::rcv_single(int clientFd) {
  return wwnet::rcv_data(clientFd, buffer, 1024, "Server");
}
