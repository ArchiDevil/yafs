set(SRC mathUnitTests.cpp
        intersectionTests.cpp
        matrixTests.cpp)
        
AddVSTest(MathLibTests ${SRC})
VSTestLinkLibraries(MathLibTests MathLib)
