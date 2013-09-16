TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += main_window.h graph_widget.h jump_to_dialog.h
FORMS += main_window.ui jump_to_dialog.ui
SOURCES += graph_widget.cpp main.cpp main_window.cpp jump_to_dialog.cpp ../fractal_laplacian.cpp

OBJECTS_DIR = obj
MOC_DIR = obj
UI_DIR = ui

