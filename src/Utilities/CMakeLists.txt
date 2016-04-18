set(SRC AppStateMachine.cpp
        AppStateMachine.h
        BlockStreamer.cpp
        BlockStreamer.h
        fnv.h
        IAppState.h
        IManager.h
        iniloader.cpp
        iniloader.h
        inputConverter.h
        logger.hpp
        observer.h
        refcounted.h
        singleton.h
        timer.cpp
        timer.h
        ut.cpp
        ut.h)

add_library(Utilities STATIC ${SRC})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
