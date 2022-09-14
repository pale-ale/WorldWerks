#pragma once
#include <cstdint>

namespace wwnet{
  enum EMessageType : std::int8_t{
    // REQ = request, RES = respones
    NONE,       // Message does not exist
    BROKEN,     // Broken packet, unreadable message type
    PLAINTXT,   // Plain text

    REQ_JOIN,   // Client requests to join the session
    RES_JOIN,   // Server responds with accept/decline
    
    REQ_MAP,    // Client requests the whole map
    RES_MAP,    // Server responds with the map data
    REQ_TSX,    // Client requests a tileset
    RES_TSX,    // Server responds with the tileset
    REQ_IMG,    // Client requests an image
    RES_IMG,    // Server responds with that image
  };

  // TODO
  template <typename T>
  T parse_message(const char *msg){

  }

  //TODO
  struct JoinRequest{

  };
};
