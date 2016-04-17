file(GLOB SRC *.cpp *.h *.hpp)

if(WIN32)
    file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/Win32/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Win32/*.h)
    list(APPEND SRC ${LS})
endif()

add_library(Utilities STATIC ${SRC})
add_subdirectory(tests)
