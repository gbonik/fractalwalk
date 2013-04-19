TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += main_window.h graph_widget.h
FORMS += main_window.ui
SOURCES += graph_widget.cpp main.cpp main_window.cpp ../fractal_laplacian.cpp

OBJECTS_DIR = obj
MOC_DIR = obj
UI_DIR = ui

