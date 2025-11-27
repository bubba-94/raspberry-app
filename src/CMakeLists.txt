add_library(${ARCHIVE}
    STATIC 
       SDL/EventManager.cpp
       SDL/Render.cpp
       SDL/Window.cpp
       SDL/Surface.cpp
       SDL/Texture.cpp
)

target_include_directories(${ARCHIVE}
    PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(${ARCHIVE}
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2_image
)