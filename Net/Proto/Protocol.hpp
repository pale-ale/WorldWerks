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

    REQ_RES,    // Client requests a resource, i.e. any file
    RES_RES,    // Server responds with that resource

    KICKED,     //The client has been removed from the server's session
  };
};
