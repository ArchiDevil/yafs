file(GLOB SRC *.cpp *.h)

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/gameStates/*.cpp)
source_group("Source Files\\Game States" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/gameStates/*.h)
source_group("Header Files\\Game States" FILES ${LS})
list(APPEND SRC ${LS})

include_directories(../../3rdparty/jsoncpp/include)

if(${EX_PLATFORM} EQUAL 64)
    link_directories($ENV{DXSDK_DIR}/lib/x64)
else()
    link_directories($ENV{DXSDK_DIR}/lib/x86)
endif()

set(GAME_LIBS
    GraphicsEngine
    System
    Utilities
    MathLib
)

add_executable(Game ${SRC})
target_link_libraries(Game ${GAME_LIBS})

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    foreach(arg ${CMAKE_CONFIGURATION_TYPES})
        file(COPY "../resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
        file(COPY "../settings.ini" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
    endforeach(arg)
endif()

if(MSVC)
   set_target_properties(Game PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(Game PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(Game PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Game PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(MSVC)
