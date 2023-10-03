#include "TooDeeEngine/Net/RPC.hpp"

void testfunction(int a, string e, float f){cout << a << ", " << e << ", " << f << ", " << endl;}

int main(){
  RPCHandler rpchandler;
  rpchandler.bindFunc("test", testfunction);
  auto ss = rpchandler.serialize("test", 10, string("test"), -1.3f);
  cout << ss << endl;
  std::stringstream s(ss);
  auto arch = cereal::XMLInputArchive(s);
  rpchandler.funcs["test"](arch);
  return 0;
}