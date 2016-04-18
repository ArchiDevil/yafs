set (SRC utilitiesUnitTests.cpp)
AddVSTest(UtilitiesTests ${SRC})
VSTestLinkLibraries(UtilitiesTests Utilities)
