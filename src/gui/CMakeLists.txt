#================
# The TaSMET GUI
#================
add_library(tasmet_gui
  mainwindow.cpp
  add_duct_dialog.cpp
  )

target_link_libraries(tasmet_gui qcustomplot)
