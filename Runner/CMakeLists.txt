project(outrunner)
cmake_minimum_required(VERSION 2.8.4)



##set(OUTRUNNERSOURCES ${CMAKE_SOURCE_DIR}/src/AnimationHandler.cpp					 					 
##					 ${CMAKE_SOURCE_DIR}/src/AStarSearch.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Bullet.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Cursor.cpp	
##					 ${CMAKE_SOURCE_DIR}/src/Enemies.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Game.cpp
##					 ## ${CMAKE_SOURCE_DIR}/src/GameObjectManager.cpp					 
##					 ${CMAKE_SOURCE_DIR}/src/GameStateEditor.cpp
##					 ${CMAKE_SOURCE_DIR}/src/GameStatePlaying.cpp
##					 ${CMAKE_SOURCE_DIR}/src/GameStateStart.cpp
##					 ${CMAKE_SOURCE_DIR}/src/GUI.cpp
##					 ## ${CMAKE_SOURCE_DIR}/src/MainMenu.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Map.cpp
##					 ${CMAKE_SOURCE_DIR}/src/MapHitbox.cpp
##					 ${CMAKE_SOURCE_DIR}/src/MapTools.cpp
##					 ${CMAKE_SOURCE_DIR}/src/MKInput.cpp
##					 ${CMAKE_SOURCE_DIR}/src/NPC.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Player.cpp
##					 ${CMAKE_SOURCE_DIR}/src/SAT.cpp
##					 ${CMAKE_SOURCE_DIR}/src/TextureManager.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Tile.cpp
##					 ## ${CMAKE_SOURCE_DIR}/src/UniformGrid.cpp
##					 ${CMAKE_SOURCE_DIR}/src/V2Tools.cpp
##					 ${CMAKE_SOURCE_DIR}/src/Weapon.cpp
##					 )

file(GLOB OUTRUNNERSOURCES src/*.h src/*.cpp)

##include_directories( ${CMAKE_SOURCE_DIR}/src
##					/usr/include
##					 )
include_directories(src)

add_library(OUTRUNNERLIBRARY OBJECT ${OUTRUNNERSOURCES})

set(SOURCESMAIN ${CMAKE_SOURCE_DIR}/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp -Ofast -funroll-loops -march=native -pthread -m64 -g")#   -ftree-vectorizer-verbose=1")  

add_executable(outrunner ${SOURCESMAIN} $<TARGET_OBJECTS:OUTRUNNERLIBRARY>)

target_link_libraries(outrunner ${OTHER_LIBRARIES} -lsfml-window -lsfml-graphics -lsfml-system -lsfml-network -luuid -Wnarrowing)

install(TARGETS outrunner RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
