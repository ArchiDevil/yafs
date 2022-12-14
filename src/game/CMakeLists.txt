set(SRC Common.h
        Game.cpp
        Game.h
        GoingHomeApplication.cpp
        GoingHomeApplication.h
        main.cpp)

set(LS  gameStates/gameState.cpp
        gameStates/MainMenuState.cpp)
source_group("Source Files\\Game States" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  gameStates/gameState.h
        gameStates/MainMenuState.h)
source_group("Header Files\\Game States" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  AI/AISmallSpirit.cpp)
source_group("Source Files\\AI" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  AI/AIBase.h
        AI/AISmallSpirit.h)
source_group("Header Files\\AI" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  AI/States/AIStateAttack.cpp
        AI/States/AIStateDodge.cpp
        AI/States/AIStateEscape.cpp
        AI/States/AIStatePursuit.cpp)
source_group("Source Files\\AI\\States" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  AI/States/AIState.h
        AI/States/AIStateAttack.h
        AI/States/AIStateDeath.h
        AI/States/AIStateDodge.h
        AI/States/AIStateEscape.h
        AI/States/AIStateIdle.h
        AI/States/AIStatePursuit.h)
source_group("Header Files\\AI\\States" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  Entities/Buffs.cpp
        Entities/Enemy.cpp
        Entities/Entity.cpp
        Entities/EntityFactory.cpp
        Entities/EntityManager.cpp
        Entities/ExperiencePoint.cpp
        Entities/LiveEntity.cpp
        Entities/MineEntity.cpp
        Entities/MineDetectorEntity.cpp
        Entities/MineTimedEntity.cpp
        Entities/PhysicsEntity.cpp
        Entities/Player.cpp
        Entities/Projectile.cpp)
source_group("Source Files\\Entities" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  Entities/Buffs.h
        Entities/Enemy.h
        Entities/Entity.h
        Entities/EntityEventManager.h
        Entities/EntityFactory.h
        Entities/EntityManager.h
        Entities/ExperiencePoint.h
        Entities/LiveEntity.h
        Entities/MineEntity.h
        Entities/MineDetectorEntity.h
        Entities/MineTimedEntity.h
        Entities/PhysicsEntity.h
        Entities/Player.h
        Entities/Projectile.h
        Entities/VisualEntity.h
        Entities/VisualStickerEntity.h)
source_group("Header Files\\Entities" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  Spells/SpellControllers.cpp
        Spells/SpellEntities.cpp
        Spells/SpellsDatabase.cpp
        Spells/SpellsDescription.cpp)
source_group("Source Files\\Spells" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  Spells/ISpellController.h
        Spells/ISpellEntity.h
        Spells/SpellControllers.h
        Spells/SpellEntities.h
        Spells/SpellsDatabase.h
        Spells/SpellsDescription.h)
source_group("Header Files\\Spells" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  LevelManagement/LevelManager.cpp)
source_group("Source Files\\Scene Management" FILES ${LS})
list(APPEND SRC ${LS})

set(LS  LevelManagement/LevelManager.h)
source_group("Header Files\\Level Management" FILES ${LS})
list(APPEND SRC ${LS})

if(${EX_PLATFORM} EQUAL 64)
    link_directories($ENV{DXSDK_DIR}/lib/x64)
else()
    link_directories($ENV{DXSDK_DIR}/lib/x86)
endif()

set(GAME_LIBS GraphicsEngine PhysicsEngine System Utilities MathLib)

add_executable(Game ${SRC})
target_link_libraries(Game ${GAME_LIBS})

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    foreach(arg ${CMAKE_CONFIGURATION_TYPES})
        file(COPY "../resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
        file(COPY "../settings.ini" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
    endforeach(arg)
endif()

if(MSVC)
   set_target_properties(Game PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   set_target_properties(Game PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   set_target_properties(Game PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Game PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(MSVC)
