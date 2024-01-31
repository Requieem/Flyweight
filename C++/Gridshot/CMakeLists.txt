# CMakeList.txt : CMake project for Gridshot, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Set the project name
project("Gridshot")

# Set C++ standard for the project
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Platform-specific header files
if(APPLE)
    set(PLATFORM_HEADERS "include/UNIXGridshot.h")
    link_directories("/usr/local/Cellar/ncurses/6.4/lib")
    message("PLATFORM: Configuring for APPLE system")
elseif(UNIX)
    set(PLATFORM_HEADERS "include/UNIXGridshot.h")
    include_directories("/usr/include/ncursesw")
    link_directories("/usr/lib")
    message("PLATFORM: Configuring for UNIX system")
elseif(WIN32)
    set(PLATFORM_HEADERS "include/WINGridshot.h")
    include_directories("C:/Users/DevDeck/Documents/Libraries/PDCurses-master")
    link_directories("C:/Users/DevDeck/Documents/Libraries/PDCurses-master/wincon")
    message("PLATFORM: Configuring for WIN system")
endif()

# Define preprocessor macros
add_definitions(-DPLATFORM_HEADER="${PLATFORM_HEADERS}")
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# Source files
set(SOURCES
    "src/Vector2Int.cpp"
    "src/CaveGenerator.cpp"
    "src/FieldObject.cpp"
    "src/GameField.cpp"
    "src/Projectile.cpp"
    "src/Soldier.cpp"
    "src/Player.cpp"
    "src/Enemy.cpp"
    "src/Node.cpp"
    "src/AStar.cpp"
    "src/Life.cpp"
    "src/Damage.cpp"
    "src/Food.cpp"
    "src/Treasure.cpp"
    "src/Item.cpp"
    "src/Score.cpp"
    "src/DisplayStrings.cpp"
    "src/Room.cpp"
    "src/Level.cpp"
    "Gridshot.cpp"
    "Gridshot.h"
)

# Add source to this project's executable
add_executable(Gridshot ${SOURCES} ${PLATFORM_HEADERS})

# Include directories
target_include_directories(Gridshot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
if(WIN32)
    target_link_libraries(Gridshot pdcurses)
elseif(UNIX)
    target_link_libraries(Gridshot ncursesw)
endif()

# TODO: Add tests and install targets if needed.