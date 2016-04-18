set(SRC mathUnitTests.cpp
        matrixTests.cpp)
        
AddVSTest(MathLibTests ${SRC})
VSTestLinkLibraries(MathLibTests MathLib)
