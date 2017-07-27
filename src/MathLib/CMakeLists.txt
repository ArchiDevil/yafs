set(SRC math.cpp
        math.h
        math/aabb2.h
        math/aabb3.h
        math/intersections.h
        math/line2.h
        math/matrix.h
        math/matrixFuncs.h
        math/oobb2.h
        math/oobb3.h
        math/plane.h
        math/quaternion.h
        math/ray3.h
        math/vector2.h
        math/vector3.h
        math/vector4.h)

add_library(MathLib STATIC ${SRC})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
