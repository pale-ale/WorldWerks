#pragma once
#include <cstdint>

namespace wwnet{
  enum EMessageType : std::int8_t{
    // REQ = request, RES = respones
    NONE,       // Message does not exist
    BROKEN,     // Broken packet, unreadable message type
    REQ_JOIN,   // Client requests to join the session
    RES_JOIN,   // Server responds with accept/decline
    PLAINTXT,   // Plain text
    REQ_MAP,    // Client requests the whole map
    RES_MAP,    // Server responds with the map data
  };

  // TODO
  template <typename T>
  T parse_message(const char *msg){

  }

  //TODO
  struct JoinRequest{

  };
};
