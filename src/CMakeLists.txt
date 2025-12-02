add_library(${ARCHIVE}
    STATIC 
       Graphics.cpp
)

target_include_directories(${ARCHIVE}
    PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(${ARCHIVE}
    PUBLIC
        SDL2::SDL2
)