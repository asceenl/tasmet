#================
# The TaSMET GUI
#================

add_library(tasmet_gui
  mainwindow.cpp
  add_duct_dialog.cpp
  add_ductbc_dialog.cpp
  about_dialog.cpp
  solver_dialog.cpp
  solver_worker.cpp
  )

target_link_libraries(tasmet_gui qcustomplot)
