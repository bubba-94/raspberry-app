add_library(${ARCHIVE}
    STATIC 
       Graphics.cpp
)

target_include_directories(${ARCHIVE}
    PUBLIC
        ${INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
)

target_link_libraries(${ARCHIVE}
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
)