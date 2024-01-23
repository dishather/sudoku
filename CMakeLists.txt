cmake_minimum_required( VERSION 2.8 )
set( CMAKE_BUILD_TYPE "Release" )

# Replace /MD with /MT for builds, otherwise link with Qt fails.
string( REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
# SET( QT_USE_QT3SUPPORT TRUE )

find_package( Qt4 REQUIRED )

include( ${QT_USE_FILE} )

message( "QT LIBRARIRES: ${QT_LIBRARIES}" )

qt4_wrap_ui( UI_HEADERS sudoku.ui )
#qt4_wrap_cpp( MOC_HEADERS  )

# we need this to be able to include headers produced by uic in our code
include_directories( ${CMAKE_BINARY_DIR} )

add_executable( sudoku WIN32 field.h field_ops.h field_ops.cpp main.cpp ${UI_HEADERS} )

target_link_libraries( sudoku ${QT_LIBRARIES} )
