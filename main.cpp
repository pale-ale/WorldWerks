#include <iostream>
#include "TooDeeEngine/3rdParty/cereal/archives/xml.hpp"
#include "TooDeeEngine/3rdParty/cereal/types/memory.hpp"
#include <string.h>

using std::cout;
using std::endl;
using std::string;

void call_me(int i){
  cout << "I was called!" << i << endl;
}

struct Entity {
  std::string name;
  int age;

  template <typename Archive>
  void serialize(Archive &ar){
    ar(name, age);
  }
  Entity(std::string name = "name", int age = 10):name{name}, age{age}{};
};

int main(){
  std::stringstream ss;
  {
    auto e = std::make_unique<Entity>("testa", 5);
    cereal::XMLOutputArchive oa(std::cout);
    oa(e);
  }
  // {
  //   auto e = std::shared_ptr<EntityTwo>{nullptr};
  //   cereal::BinaryInputArchive ia(ss);
  //   ia(e);
  //   cout << e->name << endl;
  // }
  return 0;
}