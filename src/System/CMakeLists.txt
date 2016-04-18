set(SRC InputEngine.cpp
        InputEngine.h)

if(WIN32)
    set(WIN32FILES Win32/Application.cpp
                   Win32/Application.h
                   Win32/RegistryWorker.cpp
                   Win32/RegistryWorker.h
                   Win32/System.cpp
                   Win32/System.h)
    list(APPEND SRC ${WIN32FILES})
endif()

add_library(System STATIC ${SRC})
