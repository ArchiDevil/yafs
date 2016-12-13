set (SRC physicsUnitTests.cpp)
AddVSTest(PhysicsTests ${SRC})
VSTestLinkLibraries(PhysicsTests PhysicsEngine)
