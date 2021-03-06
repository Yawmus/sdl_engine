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


# Local source files
add_library(
	INC
	src/Constants.h
	src/Entity.h
	src/Entity.cpp
	src/Game.h
	src/Game.cpp
	src/MapMaker.h
	src/MapMaker.cpp
	src/UI_Canvas.h
	src/UI_Canvas.cpp
	src/EntityManager.h
	src/EntityManager.cpp
	src/AssetManager.h
	src/AssetManager.cpp
	src/Helper.h
	src/Util.h
)

#find_package(SDL2 REQUIRED)
#find_package(SDL_gpu REQUIRED)
#find_package(SDL2_ttf REQUIRED)
#find_package(Lua REQUIRED)


# libs & headers
if(APPLE)
	set(SDL_GPU_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_gpu.framework/Headers")
	set(SDL_GPU_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_gpu.framework")

	set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2.framework/Headers")
	set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2.framework")

	set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_TTF.framework/Headers")
	set(SDL2_TTF_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/SDL2_TTF.framework")

	set(LUA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/lua_5.3/include")
	set(LUA_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/osx/lua_5.3/liblua.5.3.5.dylib")

	set(CEREAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/cereal")
	include_directories(${EXECUTABLE} ${CEREAL_INCLUDE_DIR})
	set(CXXOPTS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/cxxopts")
	include_directories(${EXECUTABLE} ${CXXOPTS_INCLUDE_DIR})
	
elseif(WIN32)
	set(SDL_GPU_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_gpu_include")
	set(SDL_GPU_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_gpu.lib")

	set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_include")
	set(SDL2_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2.lib")
	set(SDL2main_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2main.lib")

	set(SDL2_TTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_ttf_include")
	set(SDL2_TTF_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/SDL2_ttf.lib")

	set(LUA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/include")
	set(LUA_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/win/lua54.lib")

endif()


include_directories(${EXECUTABLE} ${SDL2_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${SDL2_TTF_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${SDL_GPU_INCLUDE_DIR})
include_directories(${EXECUTABLE} ${LUA_INCLUDE_DIR})


if(APPLE)
	add_executable(${EXECUTABLE} src/Main.cpp)

	target_link_libraries(${EXECUTABLE} INC)
	target_link_libraries(${EXECUTABLE} ${SDL2_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${SDL2_TTF_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${SDL_GPU_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${LUA_LIBRARY})


elseif(WIN32)
	add_executable(${EXECUTABLE} WIN32 src/Main.cpp)

	target_link_libraries(${EXECUTABLE} INC)
	target_link_libraries(${EXECUTABLE} ${SDL2_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${SDL2main_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${SDL2_TTF_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${SDL_GPU_LIBRARY})
	target_link_libraries(${EXECUTABLE} ${LUA_LIBRARY})
endif()


# At this point build folder is prepared


#print_all_variables()

