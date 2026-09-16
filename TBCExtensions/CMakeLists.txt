# Dll
cmake_minimum_required(VERSION 3.25)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(TBCExtensions LANGUAGES CXX)

option(CUSTOM_DBC "Enable custom DBC loader" OFF)
option(NOAMMO_PATCH "Removes need for ammo for ranged weapons" OFF)
option(CHARCREATIONRACE_FIX "Fixes crash if more than 21 playable races are present during character creation" ON)
option(COMBOPOINT_FIX "Fix for combo points so they work for all classes" ON)
option(OOBLUAFUNCTIONS_PATCH "Allows registering lua functions from addresses outside of wow.exe; suggested to leave it on as it's required for various other patches" ON)
option(CUSTOMPACKETS_PATCH "Enables possibility of implementing and using custom packets" OFF)
option(ITEMMODEXT_PATCH "Expands ITEM_MOD table, allowing to add and display more custom tooltip mods - see Misc.hpp for more info" OFF)
option(SPELLDESCRIPTIONEXT_PATCH "Enables possibility to add more variable arguments used in spell descriptions, like $hp, $power1, etc." OFF)
option(ACTIONBAR_LUA "Enables some additional action bar functions like 'ReplaceActionBarSpell'" OFF)
option(DEVHELPER_LUA "Various Lua functions useful in dev, like 'ReloadMap' or toggle rendering of various elements: liquids, terrain, etc." OFF)
option(UNSAFE_LEGACY_PORT "Enable incomplete WotLK-derived patches containing unverified TBC addresses" OFF)

# Warn about TODO_TBC placeholders
message(WARNING "TBC-Extensions: Many memory addresses are TODO_TBC placeholders. "
    "You MUST find the correct TBC 2.4.3 (build 8606) addresses before use. "
    "See TBC_PORTING_GUIDE.md for instructions.")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PatchConfig.hpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/PatchConfig.hpp"
)

file(GLOB_RECURSE TBCExt_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE TBCExt_includes CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

add_library(TBCExtensions SHARED
    ${TBCExt_sources}
)

target_sources(TBCExtensions PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
    FILES ${TBCExt_includes}
)

if(MSVC)
    target_compile_definitions(TBCExtensions PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

target_include_directories(TBCExtensions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
target_link_libraries(TBCExtensions PRIVATE minhook)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "DLL" FILES
    ${TBCExt_sources}
    ${TBCExt_includes}
)
