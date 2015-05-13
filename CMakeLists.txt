cmake_minimum_required(VERSION 2.8)
project(glitch-life)

set(SOURCE_FILES 
    src/bsp_util.cpp src/Entity.cpp
    src/ent_util.cpp src/gsrand.cpp
    src/Keyvalue.cpp src/model_util.cpp
    src/tex_util.cpp src/sound_util.cpp
    src/Util.cpp     src/Wad.cpp
    
    src/bsp_util.h   src/ent_data.h
    src/ent_util.h   src/Entity.h
    src/Globals.h    src/gsrand.h
    src/Keyvalue.h   src/masterList.h
    src/model_util.h src/modelList.h
    src/sentences.h  src/sound_util.h
    src/soundlists.h src/spriteList.h
    src/studio.h     src/tex_util.h
    src/Util.h       src/Wad.h )

if(MSVC)
    if(NOT MSVC_VERSION GREATER 1600) #MSVC 10
        message(FATAL_ERROR "Visual Studio 10 or greater is required")
    endif() 
    
    # Create virtual folders for VS
    source_group("Header Files\\Content Lists" FILES 
        src/masterList.h src/modelList.h src/sentences.h 
        src/soundlists.h src/spriteList.h )
    source_group("Header Files\\Utils" FILES 
        src/bsp_util.h   src/ent_util.h src/model_util.h 
        src/sound_util.h src/tex_util.h src/Util.h)
    source_group("Source Files\\Utils" FILES 
        src/bsp_util.cpp   src/ent_util.cpp src/model_util.cpp 
        src/sound_util.cpp src/tex_util.cpp src/Util.cpp)                               
    
    # compile using the static runtime
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /wd4244 /wd4018")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /wd4244 /wd4018")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-Os")
else()
    message(WARNING "Unknown compiler, no flags set")
endif()

include_directories(src)
add_executable(gsrand ${SOURCE_FILES})