add_library(glad STATIC
    GLAD/include/glad/glad.h
    GLAD/include/KHR/khrplatform.h
    GLAD/src/glad.c
)

target_include_directories(glad PUBLIC
    GLAD/include
)

set_target_properties(glad PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

file(GLOB SOURCES
    ImGui/*.cpp
    *.cpp
)

file(GLOB HEADERS
    *.h
)

include(ExternalProject)

add_executable(IGT-Project ${SOURCES} ${HEADER})

target_include_directories(IGT-Project PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/SDL3/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/SDL_ttf
                            ${CMAKE_CURRENT_SOURCE_DIR}/SDL_image
                            ${CMAKE_CURRENT_SOURCE_DIR}/ImGui
                            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(IGT-Project PRIVATE 
                        SDL2::SDL2
                        SDL2main
                        SDL2_image
                        SDL2_ttf
                        SDL2_mixer
                        ${OPENGL_LIBRARY}
                        glad
)

add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    -DUNICODE
)