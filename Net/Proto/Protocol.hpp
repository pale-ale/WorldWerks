#pragma once
#include <cstdint>

namespace wwnet{
  enum EMessageType : std::int8_t{
    BROKEN,
    REQ_JOIN,
    RES_JOIN,
    PLAINTXT,

  };

  // TODO
  template <typename T>
  T parse_message(const char *msg){

  }

  //TODO
  struct JoinRequest{

  };
};
