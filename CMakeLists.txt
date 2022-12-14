cmake_minimum_required(VERSION 2.8)

if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES Debug Release)
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
 endif()

project(YAFS)

if(MSVC60 OR MSVC70 OR MSVC71 OR MSVC80 OR CMAKE_COMPILER_2005 OR MSVC90 OR MSVC10 OR MSVC11 OR MSVC12)
    message(FATAL_ERROR "Visual Studios below 2015 are not supported")
endif()

set(RootDir ${CMAKE_CURRENT_LIST_DIR}/src)
include_directories(${RootDir})
include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty)
set(RENDERER D3D11_RENDER)

option(BUILD_TESTS "Generate projects for tests building" ON)
option(BUILD_TOOLS "Build engine tools" ON)
option(BUILD_MYGUI "Build MyGUI engine" OFF)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# The NEW behavior for this policy is to convert relative paths to absolute paths by appending the relative path to CMAKE_CURRENT_SOURCE_DIR.
cmake_policy(SET CMP0015 NEW)

# The NEW behavior for this policy is to ignore CMAKE_SHARED_LIBRARY_<Lang>_FLAGS whether it is modified or not and honor the POSITION_INDEPENDENT_CODE target property.
cmake_policy(SET CMP0018 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "x64 detected")
    set(EX_PLATFORM 64)
else()
    message(STATUS "x86 detected")
    set(EX_PLATFORM 32)
endif()

add_subdirectory(3rdparty/tinyxml2)

if(BUILD_MYGUI)
    add_definitions(-DMYGUI_STATIC)

    add_library(freetype2 STATIC IMPORTED GLOBAL)
    if(MSVC12)
        set_property(TARGET freetype2 PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/freetype2/lib/vs2013/freetype2311_d.lib)
        set_property(TARGET freetype2 PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/freetype2/lib/vs2013/freetype2311.lib)
    elseif(MSVC14)
        set_property(TARGET freetype2 PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/freetype2/lib/vs2015/freetype2311_d.lib)
        set_property(TARGET freetype2 PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/freetype2/lib/vs2015/freetype2311.lib)
    endif()

    add_library(MyGUIDirectX11Platform STATIC IMPORTED GLOBAL)
    if(MSVC12)
        set_property(TARGET MyGUIDirectX11Platform PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/mygui/lib/vs2013/Debug/MyGUI.DirectX11Platform_d.lib)
        set_property(TARGET MyGUIDirectX11Platform PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/mygui/lib/vs2013/Release/MyGUI.DirectX11Platform.lib)
    elseif(MSVC14)
        set_property(TARGET MyGUIDirectX11Platform PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/mygui/lib/vs2015/Debug/MyGUI.DirectX11Platform_d.lib)
        set_property(TARGET MyGUIDirectX11Platform PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/mygui/lib/vs2015/Release/MyGUI.DirectX11Platform.lib)
    endif()

    add_library(MyGUIEngine STATIC IMPORTED GLOBAL)
    if(MSVC12)
        set_property(TARGET MyGUIEngine PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/mygui/lib/vs2013/Debug/MyGUIEngineStatic_d.lib)
        set_property(TARGET MyGUIEngine PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/mygui/lib/vs2013/Release/MyGUIEngineStatic.lib)
    elseif(MSVC14)
        set_property(TARGET MyGUIEngine PROPERTY IMPORTED_LOCATION_DEBUG ${RootDir}/../3rdparty/mygui/lib/vs2015/Debug/MyGUIEngineStatic_d.lib)
        set_property(TARGET MyGUIEngine PROPERTY IMPORTED_LOCATION_RELEASE ${RootDir}/../3rdparty/mygui/lib/vs2015/Release/MyGUIEngineStatic.lib)
    endif()

    include_directories(3rdparty/mygui/MyGUIEngine/include)
    include_directories(3rdparty/mygui/Platforms/DirectX11/DirectX11Platform/include)
endif()

set(CMAKE_SOURCE_DIR ${OLD_CMAKE_SOURCE_DIR})

function(AddVSTest TestName Sources)
    if(NOT BUILD_TESTS OR NOT WIN32)
        return()
    endif()
    
    if(MSVC)
        # define visual studio installation dir
        if(MSVC14)
            string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS140COMNTOOLS})
        endif()
    else(MSVC)
        message(ERROR "Unable to find visual studio installation path")
    endif(MSVC)

    include_directories(${VCINSTALLDIR}/UnitTest/include)
    #include_directories($ENV{DXSDK_DIR}/include)

    link_directories(${VCINSTALLDIR}/UnitTest/lib)
    #if(${EX_PLATFORM} EQUAL 64)
    #    link_directories($ENV{DXSDK_DIR}/lib/x64)
    #else()
    #    link_directories($ENV{DXSDK_DIR}/lib/x86)
    #endif()
    
    foreach(arg ${ARGN})
        list(APPEND Sources ${arg})
    endforeach(arg)

    add_library(${TestName} SHARED ${Sources})

    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
    set_property(TARGET ${TestName} PROPERTY FOLDER "Tests")
endfunction(AddVSTest)

function(VSTestLinkLibraries TestName LinkLibraries)
    if(NOT BUILD_TESTS OR NOT WIN32)
        return()
    endif()
    foreach(arg ${ARGN})
        list(APPEND LinkLibraries ${arg})
    endforeach(arg)
    target_link_libraries(${TestName} ${LinkLibraries})
endfunction(VSTestLinkLibraries)

if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions("/wd4201 /wd4505 /wd4297 /wd4324 /D_CRT_SECURE_NO_WARNINGS /W4 /MP")
else()
    add_definitions(-std=c++11)
endif()

add_subdirectory(3rdparty/DirectXTex)
include_directories(3rdparty/DirectXTex)
set_property(TARGET DirectXTex PROPERTY FOLDER "3rdparty")

add_subdirectory(src)

if(BUILD_TESTS)
    #set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
    #add_definitions("-D_VARIADIC_MAX=10")
    #add_subdirectory(3rdparty/gtest-1.7.0)
    #set_property(TARGET gtest PROPERTY FOLDER "Tests")
    #set_property(TARGET gtest_main PROPERTY FOLDER "Tests")
    #set_property(TARGET runUnitTests PROPERTY FOLDER "Tests")
endif()
