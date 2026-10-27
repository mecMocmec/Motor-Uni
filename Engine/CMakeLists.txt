find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(DevIL REQUIRED)

add_library(Engine STATIC
    src/Application.cpp
    src/ComponentTransform.cpp
    src/GameObject.cpp
    src/Log.cpp
    src/Mesh.cpp
    src/Primitives.cpp
    src/Renderer.cpp
    src/Scene.cpp
)
target_include_directories(Engine PUBLIC include)
target_link_libraries(Engine
    PUBLIC  SDL3::SDL3 glad::glad glm::glm
    PRIVATE assimp::assimp ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES}
)
target_include_directories(Engine PRIVATE ${IL_INCLUDE_DIR})
