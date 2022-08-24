cmake_minimum_required(VERSION 3.18)
project(WorldWekrs LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

#include_directories(UI Math GameElements PRIVATE {$YOUR_DIRECTORY})

find_library (SpriteLoader spriteloader.a /home/alba/toollib/SpriteLoader)
find_package(tinytmx REQUIRED)

# add_library (UI 
#   UI/HealthBar.h   UI/HUD.h   UI/UIManager.h
#   UI/HealthBar.cpp UI/HUD.cpp UI/UIManager.cpp
# )

add_library (Inventory
  Data/InventoryStack.hpp
  Data/InventoryStack.cpp
)

add_library (TMXParser
  3rdParty/tinyxml2.hpp
  3rdParty/tinyxml2.cpp
)

add_library (UI
  UI/WBoard.hpp UI/WToken.hpp UI/UIElement.hpp UI/TokenUI.hpp UI/InputManager.hpp UI/UISystem.hpp UI/Textbox.hpp 
  UI/WBoard.cpp UI/WToken.cpp UI/UIElement.cpp UI/TokenUI.cpp UI/InputManager.cpp UI/UISystem.cpp UI/Textbox.cpp
)

add_executable(main main.cpp)

find_package(Catch2 2 REQUIRED)
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain Inventory TMXParser UI sfml-graphics sfml-system)

target_link_libraries(main UI ${SpriteLoader} TMXParser sfml-graphics sfml-window sfml-system)
