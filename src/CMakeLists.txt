#================
# The TaSMET code
#================

include_directories(
  ${PYTHON_INCLUDE_PATH}
  ${ARMADILLO_INCLUDE_DIRS}
  .
  # duct
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
  # sys
  # var
  # volume
  )

add_subdirectory(funcs)
add_subdirectory(material)
# add_subdirectory(duct)
# add_subdirectory(mech)
# add_subdirectory(seg)
# add_subdirectory(sol)
# add_subdirectory(sys)
# add_subdirectory(var)

add_library(tasmet_src tasmet_tracer.cpp tasmet_exception.cpp tasmet_assert.cpp)
target_link_libraries(tasmet_src
  # duct
  # mech
  # seg
  # sol
  # sys
  # var
  )
 
# set_source_files_properties(swig/nonlin.i
#   PROPERTIES CPLUSPLUS ON)
# swig_add_module(TaSMET python swig/nonlin.i)

# set(SWIG_MODULE_TaSMET_FILE_nonlin_EXTRA_DEPS
#   ${CMAKE_CURRENT_SOURCE_DIR}/common/swig_files/arma_numpy.i
# )
# swig_link_libraries(TaSMET tasmet_src common ${PYTHON_LIBRARIES}
#   ${ARMADILLO_LIBRARIES})

# set_source_files_properties( ${swig_generated_file_fullname}
  # PROPERTIES COMPILE_FLAGS "${SWIG_COMMON_COMPILE_FLAGS} ")
  
# Install swig files to the right place
# install(TARGETS _TaSMET
#   DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
# install(FILES ${CMAKE_BINARY_DIR}/src/TaSMET.py
#   DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
# install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/python/__init__.py
#   DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/post
#   DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/gui
#   DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})

