find_package(imgui CONFIG REQUIRED)

add_executable(Editor
    src/CameraInputHandler.cpp
    src/main.cpp
    src/EditorUI.cpp
)
target_link_libraries(Editor PRIVATE Engine imgui::imgui)

# Copia Assets junto al .exe en CADA compilación (un POST_BUILD només s'executa si es torna a enllaçar l'exe).
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:Editor>/Assets
)
add_dependencies(Editor CopyAssets)
