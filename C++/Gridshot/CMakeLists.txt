# CMakeList.txt : CMake project for Gridshot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Gridshot")

if(UNIX)
    set(PLATFORM_HEADERS "UNIXGridshot.h")
    include_directories("/usr/include/ncursesw")
    link_directories("/usr/lib")
elseif(WIN32)
    set(PLATFORM_HEADERS "WINGridshot.h")
    include_directories("C:/Users/DevDeck/Documents/Libraries/PDCurses-master")
    link_directories("C:/Users/DevDeck/Documents/Libraries/PDCurses-master/wincon")
endif()

add_definitions(-DPLATFORM_HEADER="${PLATFORM_HEADERS}")

set(SOURCES
	"src/Vector2Int.cpp"
	"src/CaveGenerator.cpp"
	"src/FieldObject.cpp"
	"src/GameField.cpp"
	"src/Projectile.cpp"
	"src/Soldier.cpp"
	"src/Enemy.cpp"
	"src/Node.cpp" 
	"src/AStar.cpp"
	"src/Life.cpp"
	"src/Damage.cpp"
	
	
	"Gridshot.cpp"
	"Gridshot.h"
)

# Add source to this project's executable.
add_executable (Gridshot ${SOURCES} ${PLATFORM_HEADERS})

target_include_directories(Gridshot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PLATFORM_HEADERS})

if(WIN32)
	target_link_libraries(Gridshot pdcurses)
elseif(UNIX)
	target_link_libraries(Gridshot ncursesw)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Gridshot PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
