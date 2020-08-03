cmake_minimum_required(VERSION 3.5)
project("Beaver")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

set(EXECUTABLE "start")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

if(APPLE)
	set(SDL_GPU_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_gpu.framework/Headers")
	set(SDL_GPU_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_gpu.framework")

	set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2.framework")
	set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2.framework/Headers")

	set(SDL2_TTF "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_TTF.framework")
	set(SDL2_TTF "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_TTF.framework/Headers")
elseif(WIN32)
	set(SDL_GPU_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_gpu.framework/Headers")
	set(SDL_GPU_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_gpu.framework")

	set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2.framework")
	set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2.framework/Headers")

	set(SDL2_TTF "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_TTF.framework")
	set(SDL2_TTF "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_TTF.framework/Headers")
endif()

#find_package(SDL2 REQUIRED)
#find_package(SDL_gpu REQUIRED)
#find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)

print_all_variables()

add_library(
	INC
	src/Constants.h
	src/Entity.h
	src/Entity.cpp
	src/Game.h
	src/Game.cpp
)


include_directories(${EXECUTABLE} ${SDL2_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${SDL2_TTF_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${SDL_GPU_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${LUA_INCLUDE_DIR})

add_executable(${EXECUTABLE} src/Main.cpp)

target_link_libraries(${EXECUTABLE} INC)
target_link_libraries(${EXECUTABLE} ${SDL2_LIBRARY})
target_link_libraries(${EXECUTABLE} ${SDL2_TTF_LIBRARY})
target_link_libraries(${EXECUTABLE} ${SDL_GPU_LIBRARY})
target_link_libraries(${EXECUTABLE} ${LUA_LIBRARY})
