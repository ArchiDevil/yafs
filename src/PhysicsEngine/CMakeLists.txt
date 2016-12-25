set(SRC
        Entity.cpp
        Entity.h
        Manager.cpp
        Manager.h
        PhysicsEngine.h
        Solver.cpp
        Solver.h
        Types.h)

add_library(PhysicsEngine ${SRC})
add_subdirectory(tests)
