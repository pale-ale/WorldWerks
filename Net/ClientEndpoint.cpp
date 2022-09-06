#include "ClientEndpoint.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cerrno>

ClientEndpoint::ClientEndpoint() {
  socketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (socketFd == -1) {
    printf("[ClientEndpoint]: Could not create a new socket. %d\n", errno);
    exit(1);
  }
}

/**
 * @brief Initialize the connection process. Since TCP uses the infamous 3-way-handshake, 
 * the connection will not be formed immediately. Use is_connected_and_buffer_empty() to determine when it is usable.
 * 
 * @param ipv4 The IPv4 to connect to.
 * @param port The port the server uses to listen to incoming connections.
 */
void ClientEndpoint::start_connecting(const char* ipv4, int port){
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = port;
  if (inet_pton(AF_INET, ipv4, &serverAddress.sin_addr) == -1) {
    printf("[Client]: Bad address: '%s'.\n", ipv4);
    exit(1);
  }
  if (connect(socketFd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
    if (errno != 115){ // errno 115: We have not finished our handshake yet.
      printf("[Client]: Could not connect to server: %d\n", errno);
      return;
    }
  }
  printf("[Client]: Connecing to: '%s:%d'...\n", ipv4, port);
}

/**
 * @brief Use this to check if a connection has been formed. Unfortunately, a non-empty buffer
 * falsifies this solution, so make sure it is empty first if it was used before.
 * 
 * @return true --- Connection is built.
 * @return false --- Not connected yet.
 */
bool ClientEndpoint::is_connected_and_buffer_empty(){
  char c;
  int ret = recv(socketFd, &c, 1, MSG_DONTWAIT|MSG_PEEK);
  if (ret == -1){
    if (errno == 11){
      return true;
    }else{
      printf("[Client]: Error checking connection status: %d\n", errno);
    }
  }
  return false;
}

/**
 * @brief Push some data towards the server.
 * 
 * @param data Strings we want to send to the server.
 */
void ClientEndpoint::send_data(const char* data){
  send(socketFd, data, strlen(data), 0);
  printf("[Client]: SND: '%s'.\n", data);
}

/**
 * @brief Fetch data sent by the server to our connection.
 * 
 * @return std::string --- The data received.
 */
std::string ClientEndpoint::rcv_data(){
  int readBytes = 1;
  std::string text = "";
  while (readBytes > 0){
    readBytes = read(socketFd, buffer, 1024);
    text += buffer;
    buffer[0] = '\0';
  }
  printf("[Client]: RCV: '%s'.\n", text.c_str());
  return text;
}

/**
 * @brief Close the connection.
 * 
 */
void ClientEndpoint::quit(){
  close(socketFd);
  printf("[Client]: Quit.");
}