include_directories(${CMAKE_SOURCE_DIR}/include)


file(GLOB_RECURSE SOURCES *.cpp *.c)
file(GLOB_RECURSE HEADERS *.hpp *.h *.inl)

add_executable(game ${SOURCES} ${HEADERS})

target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(game PRIVATE ${SDL2_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(game PRIVATE ${CMAKE_DL_LIBS})
endif()

if(UNIX AND NOT APPLE)
target_link_libraries(game PRIVATE pthread)
endif()