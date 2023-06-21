#include "../MapParser/MapParser.hpp"
#include <catch2/catch.hpp>

const char* MAP_STR = 
R""""(
<?xml version="1.0" encoding="UTF-8"?>
<map version="1.8" tiledversion="1.8.0" orientation="orthogonal" renderorder="right-down" width="25" height="14" tilewidth="16" tileheight="16" infinite="0" nextlayerid="4" nextobjectid="4">
 <tileset firstgid="1" source="Background.tsx"/>
 <tileset firstgid="2" source="Tiles.tsx"/>
 <objectgroup id="2" name="Background">
  <object id="2" name="BGSprite" gid="1" x="0" y="224" width="400" height="225"/>
 </objectgroup>
 <layer id="1" name="Tile Layer 1" width="25" height="14">
  <data encoding="csv">
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
</data>
 </layer>
 <objectgroup id="3" name="Entities">
  <object id="3" name="Party 1" x="23.563" y="192.113">
   <point/>
  </object>
 </objectgroup>
</map>
)"""";

using Catch::Matchers::Equals;

TEST_CASE("Test map loading via string"){
  tmx::MapParser mp;
  REQUIRE(mp.load_text(MAP_STR, "/home/alba/WorldWerksMap/"));
  REQUIRE(mp.map->height == 14);
  REQUIRE(mp.map->tilesets.size() == 2);
  REQUIRE(mp.map->layers.size() == 1);
  REQUIRE(mp.map->layers[0]->get_at(18,  6) == 2);
  REQUIRE(mp.map->layers[0]->get_at(25, 14) == 0);
  REQUIRE_THAT(mp.map->objectGroups[0]->name, Equals("Background"));
  REQUIRE_THAT(mp.map->objectGroups[0]->objects[0]->name, Equals("BGSprite"));
}

TEST_CASE("Test map loading via file"){
  tmx::MapParser mp;
  REQUIRE(mp.load_file("/home/alba/WorldWerksMap/ExampleMap.tmx"));
  REQUIRE(mp.map->height == 14);
  REQUIRE(mp.map->tilesets.size() == 2);
  REQUIRE(mp.map->layers.size() == 1);
  REQUIRE(mp.map->layers[0]->get_at(18,  6) == 2);
  REQUIRE(mp.map->layers[0]->get_at(25, 14) == 0);
  REQUIRE_THAT(mp.map->objectGroups[0]->name, Equals("Background"));
  REQUIRE_THAT(mp.map->objectGroups[0]->objects[0]->name, Equals("BGSprite"));
}

TEST_CASE("Test object data edit"){
  tinyxml2::XMLDocument doc;
  auto elem = doc.NewElement("Test Element");
  tmx::Object obj(elem, nullptr);
  const char* name = nullptr;
  REQUIRE_FALSE(obj.get_attribute("name", &name));
  REQUIRE_FALSE(obj.get_attribute("name", &name, true));
  elem->SetAttribute("name", "testname");
  doc.InsertFirstChild(elem);
  CHECK_THAT(obj.name, Equals(""));
  obj.update_data();
  REQUIRE(obj.get_attribute("name", &name, true));
  CHECK_THAT(obj.name, Equals("testname"));
  obj.name = "test";
  obj.id = -1;
  obj.x = 10; obj.y = 25;
  obj.commit_data();
  tinyxml2::XMLPrinter pr;
  doc.Print(&pr);
  CHECK_THAT(pr.CStr(), Equals("<Test Element name=\"test\" id=\"-1\" x=\"10\" y=\"25\"/>\n"));
}
