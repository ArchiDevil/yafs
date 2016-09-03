set(SRC FontManager.cpp
        FontManager.h
        IContextManager.h
        IMeshData.h
        IMeshManager.h
        IProgram.h
        IShaderGenerator.h
        IShaderManager.h
        ITexture.h
        ITextureManager.h
        IVertexDeclaration.h
        Material.cpp
        Material.h
        MaterialInfo.cpp
        MaterialInfo.h
        MaterialManager.cpp
        MaterialManager.h
        MiscTypes.h
        Renderer.cpp
        Renderer.h
        RenderQueue.cpp
        RenderQueue.h
        ShiftEngine.cpp
        ShiftEngine.h
        Utils.cpp
        Utils.h
        VertexTypes.cpp
        VertexTypes.h)

if(${RENDERER} STREQUAL D3D11_RENDER)
    set(DXSDK_LIBS  d3d11.lib
                    d3dcompiler.lib
                    dxguid.lib
                    dinput8.lib
                    dxgi.lib
                    Xinput.lib)
    
    set(LS  APIs/D3D11/D3D11Context.cpp
            APIs/D3D11/D3D11ContextManager.cpp
            APIs/D3D11/D3D11Mesh.cpp
            APIs/D3D11/D3D11MeshManager.cpp
            APIs/D3D11/D3D11Program.cpp
            APIs/D3D11/D3D11ShaderGenerator.cpp
            APIs/D3D11/D3D11ShaderManager.cpp
            APIs/D3D11/D3D11Texture.cpp
            APIs/D3D11/D3D11TextureManager.cpp)
    source_group("Source Files\\D3D11" FILES ${LS})
    list(APPEND SRC ${LS})
    
    set(LS  APIs/D3D11/D3D11Context.h
            APIs/D3D11/D3D11ContextManager.h
            APIs/D3D11/D3D11DepthStencil.h
            APIs/D3D11/D3D11Mesh.h
            APIs/D3D11/D3D11MeshManager.h
            APIs/D3D11/D3D11Program.h
            APIs/D3D11/D3D11RenderTarget.h
            APIs/D3D11/D3D11ShaderGenerator.h
            APIs/D3D11/D3D11ShaderManager.h
            APIs/D3D11/D3D11ShaderProgram.h
            APIs/D3D11/D3D11Texture.h
            APIs/D3D11/D3D11TextureManager.h
            APIs/D3D11/D3D11VertexDeclaration.h)
    source_group("Header Files\\D3D11" FILES ${LS})
    list(APPEND SRC ${LS})
endif()

set(LS  SceneGraph/AnimatedSpriteSceneNode.cpp
        SceneGraph/CameraFrustum.cpp
        SceneGraph/CameraSceneNode.cpp
        SceneGraph/ISceneNode.cpp
        SceneGraph/LightSceneNode.cpp
        SceneGraph/MeshSceneNode.cpp
        SceneGraph/QuadTreeNode.cpp
        SceneGraph/SceneGraph.cpp
        SceneGraph/SkySceneNode.cpp
        SceneGraph/SphericalCameraController.cpp
        SceneGraph/SpriteSceneNode.cpp)
source_group("Source Files\\Scene Graph" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  SceneGraph/AnimatedSpriteSceneNode.h
        SceneGraph/CameraFrustum.h
        SceneGraph/CameraSceneNode.h
        SceneGraph/EmptySceneNode.h
        SceneGraph/ICameraController.h
        SceneGraph/ISceneNode.h
        SceneGraph/LightSceneNode.h
        SceneGraph/MeshSceneNode.h
        SceneGraph/PlainTreeNode.h
        SceneGraph/QuadTreeNode.h
        SceneGraph/SceneGraph.h
        SceneGraph/SkySceneNode.h
        SceneGraph/SphericalCameraController.h
        SceneGraph/SpriteSceneNode.h)
source_group("Header Files\\Scene Graph" FILES ${LS})
list(APPEND SRC ${LS})

set(LS TextLib/cFont.h)
source_group("Header Files\\Text Lib" FILES ${LS})
list(APPEND SRC ${LS})
    
include_directories(../../3rdparty/tinyxml2)

add_library(GraphicsEngine STATIC ${SRC})

if(WIN32)
    target_link_libraries(GraphicsEngine ${DXSDK_LIBS})
endif()

target_link_libraries(GraphicsEngine tinyxml2 Utilities MathLib DirectXTex)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
