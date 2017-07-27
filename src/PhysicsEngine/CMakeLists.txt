set(SRC AABBCollider.cpp
        AABBCollider.h
        Entity.cpp
        Entity.h
        ICollider.h
        Manager.cpp
        Manager.h
        OOBBCollider.cpp
        OOBBCollider.h
        PhysicsEngine.h
        Solver.cpp
        Solver.h
        SphereCollider.cpp
        SphereCollider.h
        Types.h)

add_library(PhysicsEngine ${SRC})
add_subdirectory(tests)
