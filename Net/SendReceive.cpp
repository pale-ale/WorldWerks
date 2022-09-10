#include "SendReceive.hpp"

#include <memory.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "../Util/Util.hpp"
#include "Proto/Protocol.hpp"

#define MSG_LENGTH_BYTES 8
#define MSG_TYPE_BYTES 4

namespace wwnet {

/**
 * @brief Send some data towards the recipient.
 *
 * @param data bytes we want to send.
 */
void send_data(int socketFd, EMessageType msgType, const char* data, const char* epName) {
  if (msgType == EMessageType::NONE) {
    printf("[%s]: Cannot send messages of type NONE.\n", epName);
    return;
  }
  std::string msgLenStr = std::to_string(strlen(data));
  zfill(MSG_LENGTH_BYTES, msgLenStr);

  std::string msgTypeStr = std::to_string(msgType);
  zfill(MSG_TYPE_BYTES, msgTypeStr);

  std::string msg = msgTypeStr + msgLenStr + data;
  send(socketFd, msg.c_str(), msg.length(), 0);
  printf("[%s]: SND: '%s' + '%s' + '%s'.\n", epName, msgTypeStr.c_str(),
         msgLenStr.c_str(), data);
}

/**
 * @brief Fetch data sent to our connection.
 *
 * @return std::pair<EMessageType, std::string> --- The message type and data received.
 * EMessageType will be NONE if there is no message.
 */
std::pair<EMessageType, std::string> rcv_data(int socketFd, char* buffer, int bufSize,
                                              const char* epName) {
  // Extract the message type
  std::string msgTypeStr(MSG_TYPE_BYTES, '\0');
  int readBytes = read(socketFd, &msgTypeStr[0], MSG_TYPE_BYTES);
  if (readBytes <= 0) {
    return {EMessageType::NONE, ""};
  }
  if (readBytes != MSG_TYPE_BYTES) {
    printf("[%s]: Received message without message type.\n", epName);
    return {EMessageType::BROKEN, ""};
  }
  EMessageType msgType = (EMessageType)std::stoi(msgTypeStr);

  // Extract the payload length
  std::string msgLenStr(MSG_LENGTH_BYTES, '\0');
  readBytes = read(socketFd, &msgLenStr[0], MSG_LENGTH_BYTES);
  if (readBytes != MSG_LENGTH_BYTES) {
    printf("[%s]: Received message without length.\n", epName);
    return {EMessageType::BROKEN, ""};
  }
  int msgLength = std::stoi(msgLenStr);

  // Read the payload
  readBytes = 1;
  std::string text = "";
  while (readBytes > 0 && msgLength > 0) {
    readBytes = read(socketFd, buffer, std::min<int>(msgLength, bufSize));
    msgLength -= readBytes;
    text += buffer;
    buffer[0] = '\0';
  }
  printf("[%s]: RCV: '%s' + '%s' + '%s'.\n", epName, msgTypeStr.c_str(),
         msgLenStr.c_str(), text.c_str());
  return {msgType, text};
}

/**
 * @brief Check if the socket was created and is not terminated.
 * Does not detect quiet disconnects.
 *
 * @return true --- The socket is up.
 * @return false --- Socket is not connected / was sut down.
 */
bool is_socket_up(int socketFd) {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(socketFd, SOL_SOCKET, SO_ERROR, &error, &len);
  int result = error | retval;
  if (result != 0) {
    printf("[Client]: Non-Zero socket connection status: %d (error) | %d (status)\n",
           error, retval);
  }
  return (error | retval) == 0;
}
};  // namespace wwnet
