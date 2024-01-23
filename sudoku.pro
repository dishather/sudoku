TARGET  = sudoku
SOURCES = main.cpp field_ops.cpp sudoku.cpp solver_thread.cpp
HEADERS = field_ops.h sudoku.h solver_thread.h
FORMS   = sudoku.ui
CONFIG += qt warn_on release exceptions no_batch windows c++11
CONFIG -= incremental debug_and_release debug_and_release_targets
QT     += core widgets
RC_FILE = icon.rc

