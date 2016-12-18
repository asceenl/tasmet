#================
# The TaSMET code
#================

include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${ARMADILLO_INCLUDE_DIRS}
  .
  protobuf
  duct
  # duct/cell
  # duct/connectors
  # duct/eq
  # duct/geom
  # duct/geom/grid
  # duct/drag
  # duct/heat
  # mech
  # python
  # seg
  # sol
  sys
  solver
  # var
  # volume
  )

add_library(tasmet_src
  tasmet_tracer.cpp
  tasmet_exception.cpp
  tasmet_assert.cpp
  tasmet_pyeval.cpp

  duct/grid.cpp
  duct/geom.cpp
  duct/duct.cpp

  funcs/bessel.cpp
  funcs/cbessj.cpp
  funcs/rottfuncs.cpp
  funcs/skewsine.cpp
  funcs/sph_j.cpp

  solver/solver.cpp
  solver/system.cpp
  solver/newton_raphson.cpp
  solver/brent.cpp

  material/gas.cpp
  material/air.cpp
  material/helium.cpp
  material/nitrogen.cpp
  material/solid.cpp
  material/adiabatictemp.cpp

  # sys/enginesystem.cpp
  sys/globalconf.cpp
  sys/tasmet_variable.cpp
  sys/jaccol.cpp
  sys/jacobian.cpp
  sys/jacrow.cpp
  sys/tasystem.cpp
  sys/triplets.cpp
)

add_subdirectory(gui) 
add_subdirectory(protobuf)
