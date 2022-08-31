#include "Net/ServerEndpoint.hpp"
#include <chrono>
#include <thread>

ServerEndpoint serverEp("127.0.0.1", 12345);

int main(){
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  serverEp.accept_connection();
}
