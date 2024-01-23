#include <QApplication>
// #include <QTextCodec>
#include "sudoku.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    // QTextCodec::setCodecForTr( QTextCodec::codecForName( "CP1251" ) );
    // QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    SudokuDialog su;
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    su.show();
    return app.exec(); 
}

