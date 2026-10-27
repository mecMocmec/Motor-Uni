find_package(imgui CONFIG REQUIRED)

add_executable(Editor
    src/main.cpp
    src/EditorUI.cpp
)
target_link_libraries(Editor PRIVATE Engine imgui::imgui)

# Copia los recursos junto al .exe
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:Editor>/Assets
)
