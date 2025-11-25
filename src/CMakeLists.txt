add_library(${ARCHIVE}
    STATIC 
       EventManager.cpp
       Renderer.cpp
       Window.cpp
)

target_include_directories(${ARCHIVE} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${ARCHIVE} PUBLIC SDL2::SDL2)

message("-- ${ARCHIVE} linked.")