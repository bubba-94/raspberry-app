add_library(${ARCHIVE}
    STATIC 
       EventManager.cpp
       Render.cpp
       Window.cpp
       Surface.cpp
       Texture.cpp
)

target_include_directories(${ARCHIVE} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${ARCHIVE} PUBLIC SDL2::SDL2)

message("-- ${ARCHIVE} linked.")