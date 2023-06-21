cmake_minimum_required(VERSION 3.18)
project(WorldWekrs LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_library (SpriteLoader spriteloader.a /home/alba/toollib/SpriteLoader)

find_package(tinytmx REQUIRED)
find_package(Catch2 2 REQUIRED)

##### Engine Libraries

set(ENGINE_PATH ../TooDeeEngine/)
set(LIB ${ENGINE_PATH}/UI)
add_library (Engine_UI
  ${LIB}/UIElement.hpp ${LIB}/InputManager.hpp ${LIB}/UISystem.hpp ${LIB}/WTextbox.hpp ${LIB}/WBar.hpp ${LIB}/WButton.hpp
  ${LIB}/UIElement.cpp ${LIB}/InputManager.cpp ${LIB}/UISystem.cpp ${LIB}/WTextbox.cpp ${LIB}/WBar.cpp ${LIB}/WButton.cpp
)

set(LIB ${ENGINE_PATH}/Net)
add_library (Client
  ${LIB}/ClientEndpoint.hpp ${LIB}/SendReceive.hpp
  ${LIB}/ClientEndpoint.cpp ${LIB}/SendReceive.cpp
)
add_library (Server 
  ${LIB}/ServerEndpoint.hpp ${LIB}/SendReceive.hpp 
  ${LIB}/ServerEndpoint.cpp ${LIB}/SendReceive.cpp
)

add_library (Scene 
  Scene/MainMenuScene.hpp 
  Scene/MainMenuScene.cpp
)

set(LIB ${ENGINE_PATH}/Util)
add_library (Util 
  ${LIB}/Util.hpp
  ${LIB}/Util.cpp
)

set(LIB ${ENGINE_PATH}/Storage)
add_library (Storage
  ${LIB}/LiveStorage.hpp
  ${LIB}/LiveStorage.cpp
)

# set(LIB ${ENGINE_PATH}/Components)
# add_library (Components
#   ${LIB}/LiveStorage.hpp
#   ${LIB}/LiveStorage.cpp
# )

set(LIB ${ENGINE_PATH}/3rdParty)
add_library (TMXParser
  ${LIB}/tinyxml2.hpp MapParser/MapParser.hpp MapParser/Map.hpp MapParser/Tileset.hpp
  ${LIB}/tinyxml2.cpp MapParser/MapParser.cpp MapParser/Map.cpp MapParser/Tileset.cpp
)

##### Project Libraries

add_library (Data
  Data/InventoryStack.hpp Data/DataChangeSource.hpp
  Data/InventoryStack.cpp Data/DataChangeSource.cpp
)

add_library (UI
  UI/WBoard.hpp UI/TokenUI.hpp UI/UIComponents/WToken.hpp
  UI/WBoard.cpp UI/TokenUI.cpp UI/UIComponents/WToken.cpp
)

add_library (Tabletop
  Tabletop/Board.hpp Tabletop/Token.hpp
  Tabletop/Board.cpp Tabletop/Token.cpp
)

add_executable (server server_main.cpp)
add_executable (client client_main.cpp)
add_executable (tests test.cpp)

target_link_libraries(server 
  TMXParser
  Server 
  Util Data Storage
  fmt
)

target_link_libraries(client 
  Scene
  ${SpriteLoader}
  Tabletop 
  TMXParser UI Engine_UI
  Server Client 
  Util Data Storage
  fmt
  sfml-graphics sfml-window sfml-system 
)

target_link_libraries(tests PRIVATE 
  Catch2::Catch2WithMain
  TMXParser UI Tabletop Engine_UI 
  Server Client
  Util Data Storage
  fmt
  sfml-graphics sfml-window sfml-system
)
