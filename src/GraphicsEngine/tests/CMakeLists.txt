set(SOURCES CommonTests.cpp
            TestCommon.h
            TextureManagerTests.cpp)

AddVSTest(GraphicsEngineUnitTests ${SOURCES})
VSTestLinkLibraries(GraphicsEngineUnitTests GraphicsEngine)
