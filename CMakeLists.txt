cmake_minimum_required(VERSION 3.0)

project(simulant VERSION 0.0.1)

# Prepare doctest for other targets to use
find_package(doctest REQUIRED)
include(doctest)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include") 

set(SRCS
    src/controllers/ant.cpp
    src/controllers/game.cpp
    src/controllers/random.cpp

    src/models/space.cpp
    src/models/grid.cpp

    src/view/view.cpp
    src/view/color.cpp
    src/view/circle.cpp
    src/view/update.cpp
    src/view/hud.cpp
    src/view/render.cpp
)

set(HEADERS
    src/controllers/ant.hpp
    src/controllers/game.hpp
    src/controllers/random.hpp

    src/models/space.hpp
    src/models/grid.hpp
    src/models/grid.tpp

    src/view/view.hpp
    src/view/color.hpp
    src/view/hud.hpp
)

set(TESTS
    tests/tests.cpp
    
    tests/controllers/game.cpp

    tests/models/space.cpp
)

set(LIBS
    SDL2
    SDL2_ttf
    SDL2_image
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

add_executable(simulant src/main.cpp ${SRCS} ${HEADERS})
target_compile_features(simulant PRIVATE cxx_std_17)
target_include_directories(simulant PUBLIC src/)
target_link_libraries(simulant ${LIBS})

# Make test executable

# tests
add_executable(run_tests ${TESTS} ${SRCS} ${HEADERS})
target_compile_features(run_tests PRIVATE cxx_std_17)
target_include_directories(run_tests PUBLIC src/)
target_link_libraries(run_tests PRIVATE doctest::doctest ${LIBS})
