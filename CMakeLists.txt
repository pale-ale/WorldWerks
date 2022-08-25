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
  3rdParty/tinyxml2.hpp MapParser/Map.hpp
  3rdParty/tinyxml2.cpp MapParser/Map.cpp
)

add_library (UI
  UI/WBoard.hpp UI/WToken.hpp UI/UIElement.hpp UI/TokenUI.hpp UI/InputManager.hpp UI/UISystem.hpp UI/Textbox.hpp 
  UI/WBoard.cpp UI/WToken.cpp UI/UIElement.cpp UI/TokenUI.cpp UI/InputManager.cpp UI/UISystem.cpp UI/Textbox.cpp
)

add_library (Tabletop
  Tabletop/Board.hpp Tabletop/Token.hpp
  Tabletop/Board.cpp Tabletop/Token.cpp
)

add_executable(main main.cpp)
target_link_libraries(main UI ${SpriteLoader} TMXParser Data Tabletop sfml-graphics sfml-window sfml-system)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain Data Tabletop TMXParser UI sfml-graphics sfml-system)

target_link_libraries(UI Tabletop)
target_link_libraries(Tabletop Data)

