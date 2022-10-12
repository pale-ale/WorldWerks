#include "ClientEndpoint.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>

#include "../Util/Log.hpp"

ClientEndpoint::ClientEndpoint() {
  socketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (socketFd == -1) {
    LOGERR("ClientEndpoint",
           fmt::format("Could not create a new socket: Errno {}.", errno));
    exit(1);
  }
}

/**
 * @brief Initialize the connection process. Since TCP uses the infamous 3-way-handshake,
 * the connection will not be formed immediately. Use is_connected_and_buffer_empty() to
 * determine when it is usable.
 *
 * @param ipv4 The IPv4 to connect to.
 * @param port The port the server uses to listen to incoming connections.
 */
void ClientEndpoint::start_connecting(const char* ipv4, int port) {
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = port;
  if (inet_pton(AF_INET, ipv4, &serverAddress.sin_addr) != 1) {
    LOGERR("Client", fmt::format("Bad address: '{}'.", ipv4));
    return;
  }
  if (connect(socketFd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
    if (errno != 115) {  // errno 115: We have not finished our handshake yet.
      LOGERR("Client", fmt::format("Could not connect to server: {}.", errno));
      return;
    }
  }
  LOGINF("Client", fmt::format("Connecing to: '{}:{}'...", ipv4, port));
}

/**
 * @brief Use this to check if a connection has been formed. Unfortunately, a non-empty
 * buffer falsifies this solution, so make sure it is empty first if it was used before.
 *
 * @return true --- Connection is built.
 * @return false --- Not connected yet.
 */
bool ClientEndpoint::is_connected_and_buffer_empty() {
  char c;
  int ret = recv(socketFd, &c, 1, MSG_DONTWAIT | MSG_PEEK);
  if (ret == -1) {
    if (errno == 11) {
      return true;
    } else {
      LOGERR("Client", fmt::format("Error checking connection status: {}", errno));
    }
  }
  return false;
}

/**
 * @brief Send data to the server.
 *
 * @param msgType The message type, i.e. if a player joins, edits a token, etc.
 * @param data The data to send.
 */
void ClientEndpoint::send_data(wwnet::EMessageType msgType, const char* data) {
  wwnet::send_data(socketFd, msgType, data, "Client");
}

/**
 * @brief Read data from the server.
 *
 * @return std::pair<wwnet::EMessageType, std::string> --- The message type and the
 * contents.
 */
std::pair<wwnet::EMessageType, std::string> ClientEndpoint::rcv_data() {
  return wwnet::rcv_data(socketFd, buffer, bufferSize, "Client");
}

/**
 * @brief Request the whole map from the server as XML data.
 * Use receive_map() to try and read the server's response.
 *
 * @return true: Request successful.
 * @return false: Request failed.
 */
bool ClientEndpoint::request_map() {
  send_data(wwnet::EMessageType::REQ_MAP, "");
  return true;
}

/**
 * @brief Process incoming messages from the server.
 *
 */
void ClientEndpoint::digest_incoming() {
  auto [msgType, msgData] = rcv_data();
  while (msgType != wwnet::NONE) {
    for (auto&& cb : callbacks[msgType]) {
      cb(msgData);
    }
    std::tie(msgType, msgData) = rcv_data();
  }
}

/**
 * @brief Close the connection.
 *
 */
void ClientEndpoint::quit() {
  close(socketFd);
  LOGINF("Client", fmt::format("Quit."));
}
