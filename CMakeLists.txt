cmake_minimum_required(VERSION 3.0)
project(glitch-life)

if(MSVC)
    if(NOT MSVC_VERSION GREATER 1600) #MSVC 10
        message(FATAL "Visual Studio 10 or greater is required")
    endif()
else()
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -Wno-unused")
endif()

include_directories(alerter)
add_executable(gsrand
    alerter/bsp_util.cpp
    alerter/Entity.cpp
    alerter/ent_util.cpp
    alerter/gsrand.cpp
    alerter/Keyvalue.cpp
    alerter/model_util.cpp
    alerter/sound_util.cpp
    alerter/tex_util.cpp
    alerter/Util.cpp
    alerter/Wad.cpp
)
