cmake_minimum_required(VERSION 3.18)
project(WorldWekrs LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_library (SpriteLoader spriteloader.a /home/alba/toollib/SpriteLoader)
find_package(tinytmx REQUIRED)
find_package(Catch2 2 REQUIRED)

add_library (Data
  Data/InventoryStack.hpp Data/DataChangeSource.hpp
  Data/InventoryStack.cpp Data/DataChangeSource.cpp
)

add_library (TMXParser
  3rdParty/tinyxml2.hpp MapParser/Map.hpp MapParser/Tileset.hpp
  3rdParty/tinyxml2.cpp MapParser/Map.cpp MapParser/Tileset.cpp
)

add_library (UI
  UI/WBoard.hpp UI/WToken.hpp UI/UIElement.hpp UI/TokenUI.hpp UI/InputManager.hpp UI/UISystem.hpp UI/WTextbox.hpp UI/WBar.hpp UI/WButton.hpp
  UI/WBoard.cpp UI/WToken.cpp UI/UIElement.cpp UI/TokenUI.cpp UI/InputManager.cpp UI/UISystem.cpp UI/WTextbox.cpp UI/WBar.cpp UI/WButton.cpp
)

add_library (Tabletop
  Tabletop/Board.hpp Tabletop/Token.hpp
  Tabletop/Board.cpp Tabletop/Token.cpp
)

add_library (Client
  Net/ClientEndpoint.hpp Net/SendReceive.hpp
  Net/ClientEndpoint.cpp Net/SendReceive.cpp
)

add_library (Server 
  Net/ServerEndpoint.hpp Net/SendReceive.hpp 
  Net/ServerEndpoint.cpp Net/SendReceive.cpp
)

add_library (Scene 
  Scene/MainMenuScene.hpp 
  Scene/MainMenuScene.cpp
)

add_library (Util 
  Util/Util.hpp
  Util/Util.cpp
)

add_library (Storage
  Storage/LiveStorage.hpp
  Storage/LiveStorage.cpp
)

add_executable (client client_main.cpp)
target_link_libraries(client 
  Scene
  ${SpriteLoader}
  TMXParser UI 
  Server Client 
  Util Tabletop Data Storage
  fmt
  sfml-graphics sfml-window sfml-system 
)

add_executable (server server_main.cpp)
target_link_libraries(server 
  TMXParser
  Server 
  Util Storage
  fmt
)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain
  Util TMXParser UI 
  Server Client
  Util Tabletop Data Storage
  fmt
  sfml-graphics sfml-window sfml-system
)
