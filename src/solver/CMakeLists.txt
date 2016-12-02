include_directories(${PROJECT_SOURCE_DIR}/src/solver)
add_library(solver
  solver.cpp
  system.cpp
  newton_raphson.cpp
  brent.cpp
  )
