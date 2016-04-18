set(SRC math.h
        math/AABB.cpp
        math/AABB.h
        math/intersections.cpp
        math/intersections.h
        math/matrix.h
        math/matrixFuncs.h
        math/OOBB.cpp
        math/OOBB.h
        math/plane.h
        math/quaternion.h
        math/ray.h
        math/vector2.h
        math/vector3.h
        math/vector4.h
        math/vectorsFuncs.h)

add_library(MathLib STATIC ${SRC})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
