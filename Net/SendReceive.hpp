#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "Proto/Protocol.hpp"

namespace wwnet {
extern void send_data(int socketFd, EMessageType msgType, const char* data,
                      const char* epName);
extern std::pair<EMessageType, std::string> rcv_data(int socketFd, char* buffer, int bufSize,
                                                     const char* epName);
extern bool is_socket_up(int socketFd);
};  // namespace wwnet
