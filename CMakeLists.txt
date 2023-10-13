cmake_minimum_required(VERSION 3.10)
project(n_queen_problem_solver)

include_directories(n_queen_problem_sovler ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(n_queen_problem_solver
    main.cpp
    src/state.h
    src/state.cpp
    src/position.h
    src/position.cpp
    src/hill_climbing_searcher.h
    src/hill_climbing_searcher.cpp
    src/genetic_solver.h
    src/genetic_solver.cpp
)
