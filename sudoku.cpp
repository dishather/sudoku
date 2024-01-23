#include "sudoku.h"
#include "field_ops.h"
#include <QTimer>

namespace {

//
void tick_field( sudoku_field_t &f, int x, int y )
{
    unsigned char c = f.field[x][y];
    if( c == 0 )
    {
        c = '1';
    }
    else if( c == '9' )
    {
        c = 0;
    }
    else
    {
        ++c;
    }
    f.field[x][y] = c;
}

//
void set_button( QToolButton *bt, unsigned char src, unsigned char solved,
                int numsol )
{
    QString txt;
    if( src == 0 )
    {
        if( ( numsol > 0 ) && ( solved != 0 ) )
        {
            txt = QString( "%1").arg( static_cast<char>(solved) );
        }
        bt->setStyleSheet( QString() );
    }
    else
    {
        txt = QString( "[%1]").arg(static_cast<char>(src));
        bt->setStyleSheet( "QToolButton { color: black; "
            "background: qconicalgradient(cx:0, cy:0, angle:135, "
            "stop:0 rgba(255, 255, 0, 69), stop:0.375 rgba(255, 255, 0, 69), "
            "stop:0.423533 rgba(251, 255, 0, 145), stop:0.45 "
            "rgba(247, 255, 0, 208), stop:0.477581 "
            "rgba(255, 244, 71, 130), stop:0.518717 "
            "rgba(255, 218, 71, 130), stop:0.55 "
            "rgba(255, 255, 0, 255), stop:0.57754 rgba(255, 203, 0, 130), "
            "stop:0.625 rgba(255, 255, 0, 69), stop:1 rgba(255, 255, 0, 69)) }"
        );
    }
    bt->setText( txt );
}

} // anonymous namespace ends

//
SudokuDialog::SudokuDialog():
    ui(),
    field(),
    solved_field(),
    num_solutions( 0 ),
    solver( field, solved_field, num_solutions )
{
    memset( &field.field, 0, 81 );
    solved_field = field;
    ui.setupUi( this );
    bt[0][0] = ui.b_00;
    bt[1][0] = ui.b_10;
    bt[2][0] = ui.b_20;
    bt[3][0] = ui.b_30;
    bt[4][0] = ui.b_40;
    bt[5][0] = ui.b_50;
    bt[6][0] = ui.b_60;
    bt[7][0] = ui.b_70;
    bt[8][0] = ui.b_80;
    bt[0][1] = ui.b_01;
    bt[1][1] = ui.b_11;
    bt[2][1] = ui.b_21;
    bt[3][1] = ui.b_31;
    bt[4][1] = ui.b_41;
    bt[5][1] = ui.b_51;
    bt[6][1] = ui.b_61;
    bt[7][1] = ui.b_71;
    bt[8][1] = ui.b_81;
    bt[0][2] = ui.b_02;
    bt[1][2] = ui.b_12;
    bt[2][2] = ui.b_22;
    bt[3][2] = ui.b_32;
    bt[4][2] = ui.b_42;
    bt[5][2] = ui.b_52;
    bt[6][2] = ui.b_62;
    bt[7][2] = ui.b_72;
    bt[8][2] = ui.b_82;
    bt[0][3] = ui.b_03;
    bt[1][3] = ui.b_13;
    bt[2][3] = ui.b_23;
    bt[3][3] = ui.b_33;
    bt[4][3] = ui.b_43;
    bt[5][3] = ui.b_53;
    bt[6][3] = ui.b_63;
    bt[7][3] = ui.b_73;
    bt[8][3] = ui.b_83;
    bt[0][4] = ui.b_04;
    bt[1][4] = ui.b_14;
    bt[2][4] = ui.b_24;
    bt[3][4] = ui.b_34;
    bt[4][4] = ui.b_44;
    bt[5][4] = ui.b_54;
    bt[6][4] = ui.b_64;
    bt[7][4] = ui.b_74;
    bt[8][4] = ui.b_84;
    bt[0][5] = ui.b_05;
    bt[1][5] = ui.b_15;
    bt[2][5] = ui.b_25;
    bt[3][5] = ui.b_35;
    bt[4][5] = ui.b_45;
    bt[5][5] = ui.b_55;
    bt[6][5] = ui.b_65;
    bt[7][5] = ui.b_75;
    bt[8][5] = ui.b_85;
    bt[0][6] = ui.b_06;
    bt[1][6] = ui.b_16;
    bt[2][6] = ui.b_26;
    bt[3][6] = ui.b_36;
    bt[4][6] = ui.b_46;
    bt[5][6] = ui.b_56;
    bt[6][6] = ui.b_66;
    bt[7][6] = ui.b_76;
    bt[8][6] = ui.b_86;
    bt[0][7] = ui.b_07;
    bt[1][7] = ui.b_17;
    bt[2][7] = ui.b_27;
    bt[3][7] = ui.b_37;
    bt[4][7] = ui.b_47;
    bt[5][7] = ui.b_57;
    bt[6][7] = ui.b_67;
    bt[7][7] = ui.b_77;
    bt[8][7] = ui.b_87;
    bt[0][8] = ui.b_08;
    bt[1][8] = ui.b_18;
    bt[2][8] = ui.b_28;
    bt[3][8] = ui.b_38;
    bt[4][8] = ui.b_48;
    bt[5][8] = ui.b_58;
    bt[6][8] = ui.b_68;
    bt[7][8] = ui.b_78;
    bt[8][8] = ui.b_88;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(250);
}

SudokuDialog::~SudokuDialog()
{
}

void SudokuDialog::process_click( int x, int y )
{
    do
    {
        tick_field( field, x, y );
    }
    while( ! is_field_valid( field ) );
    update();

    solver.set_fresh();
    if(! solver.isRunning() )
    {
        solver.wait( 250 );
        solver.start( QThread::LowestPriority );
    }
}

void SudokuDialog::on_b_00_clicked()
{
    process_click( 0, 0 );
}

void SudokuDialog::on_b_10_clicked()
{
    process_click( 1, 0 );
}

void SudokuDialog::on_b_20_clicked()
{
    process_click( 2, 0 );
}

void SudokuDialog::on_b_30_clicked()
{
    process_click( 3, 0 );
}

void SudokuDialog::on_b_40_clicked()
{
    process_click( 4, 0 );
}

void SudokuDialog::on_b_50_clicked()
{
    process_click( 5, 0 );
}

void SudokuDialog::on_b_60_clicked()
{
    process_click( 6, 0 );
}

void SudokuDialog::on_b_70_clicked()
{
    process_click( 7, 0 );
}

void SudokuDialog::on_b_80_clicked()
{
    process_click( 8, 0 );
}

void SudokuDialog::on_b_01_clicked()
{
    process_click( 0, 1 );
}

void SudokuDialog::on_b_11_clicked()
{
    process_click( 1, 1 );
}

void SudokuDialog::on_b_21_clicked()
{
    process_click( 2, 1 );
}

void SudokuDialog::on_b_31_clicked()
{
    process_click( 3, 1 );
}

void SudokuDialog::on_b_41_clicked()
{
    process_click( 4, 1 );
}

void SudokuDialog::on_b_51_clicked()
{
    process_click( 5, 1 );
}

void SudokuDialog::on_b_61_clicked()
{
    process_click( 6, 1 );
}

void SudokuDialog::on_b_71_clicked()
{
    process_click( 7, 1 );
}

void SudokuDialog::on_b_81_clicked()
{
    process_click( 8, 1 );
}

void SudokuDialog::on_b_02_clicked()
{
    process_click( 0, 2 );
}

void SudokuDialog::on_b_12_clicked()
{
    process_click( 1, 2 );
}

void SudokuDialog::on_b_22_clicked()
{
    process_click( 2, 2 );
}

void SudokuDialog::on_b_32_clicked()
{
    process_click( 3, 2 );
}

void SudokuDialog::on_b_42_clicked()
{
    process_click( 4, 2 );
}

void SudokuDialog::on_b_52_clicked()
{
    process_click( 5, 2 );
}

void SudokuDialog::on_b_62_clicked()
{
    process_click( 6, 2 );
}

void SudokuDialog::on_b_72_clicked()
{
    process_click( 7, 2 );
}

void SudokuDialog::on_b_82_clicked()
{
    process_click( 8, 2 );
}

void SudokuDialog::on_b_03_clicked()
{
    process_click( 0, 3 );
}

void SudokuDialog::on_b_13_clicked()
{
    process_click( 1, 3 );
}

void SudokuDialog::on_b_23_clicked()
{
    process_click( 2, 3 );
}

void SudokuDialog::on_b_33_clicked()
{
    process_click( 3, 3 );
}

void SudokuDialog::on_b_43_clicked()
{
    process_click( 4, 3 );
}

void SudokuDialog::on_b_53_clicked()
{
    process_click( 5, 3 );
}

void SudokuDialog::on_b_63_clicked()
{
    process_click( 6, 3 );
}

void SudokuDialog::on_b_73_clicked()
{
    process_click( 7, 3 );
}

void SudokuDialog::on_b_83_clicked()
{
    process_click( 8, 3 );
}

void SudokuDialog::on_b_04_clicked()
{
    process_click( 0, 4 );
}

void SudokuDialog::on_b_14_clicked()
{
    process_click( 1, 4 );
}

void SudokuDialog::on_b_24_clicked()
{
    process_click( 2, 4 );
}

void SudokuDialog::on_b_34_clicked()
{
    process_click( 3, 4 );
}

void SudokuDialog::on_b_44_clicked()
{
    process_click( 4, 4 );
}

void SudokuDialog::on_b_54_clicked()
{
    process_click( 5, 4 );
}

void SudokuDialog::on_b_64_clicked()
{
    process_click( 6, 4 );
}

void SudokuDialog::on_b_74_clicked()
{
    process_click( 7, 4 );
}

void SudokuDialog::on_b_84_clicked()
{
    process_click( 8, 4 );
}

void SudokuDialog::on_b_05_clicked()
{
    process_click( 0, 5 );
}

void SudokuDialog::on_b_15_clicked()
{
    process_click( 1, 5 );
}

void SudokuDialog::on_b_25_clicked()
{
    process_click( 2, 5 );
}

void SudokuDialog::on_b_35_clicked()
{
    process_click( 3, 5 );
}

void SudokuDialog::on_b_45_clicked()
{
    process_click( 4, 5 );
}

void SudokuDialog::on_b_55_clicked()
{
    process_click( 5, 5 );
}

void SudokuDialog::on_b_65_clicked()
{
    process_click( 6, 5 );
}

void SudokuDialog::on_b_75_clicked()
{
    process_click( 7, 5 );
}

void SudokuDialog::on_b_85_clicked()
{
    process_click( 8, 5 );
}

void SudokuDialog::on_b_06_clicked()
{
    process_click( 0, 6 );
}

void SudokuDialog::on_b_16_clicked()
{
    process_click( 1, 6 );
}

void SudokuDialog::on_b_26_clicked()
{
    process_click( 2, 6 );
}

void SudokuDialog::on_b_36_clicked()
{
    process_click( 3, 6 );
}

void SudokuDialog::on_b_46_clicked()
{
    process_click( 4, 6 );
}

void SudokuDialog::on_b_56_clicked()
{
    process_click( 5, 6 );
}

void SudokuDialog::on_b_66_clicked()
{
    process_click( 6, 6 );
}

void SudokuDialog::on_b_76_clicked()
{
    process_click( 7, 6 );
}

void SudokuDialog::on_b_86_clicked()
{
    process_click( 8, 6 );
}

void SudokuDialog::on_b_07_clicked()
{
    process_click( 0, 7 );
}

void SudokuDialog::on_b_17_clicked()
{
    process_click( 1, 7 );
}

void SudokuDialog::on_b_27_clicked()
{
    process_click( 2, 7 );
}

void SudokuDialog::on_b_37_clicked()
{
    process_click( 3, 7 );
}

void SudokuDialog::on_b_47_clicked()
{
    process_click( 4, 7 );
}

void SudokuDialog::on_b_57_clicked()
{
    process_click( 5, 7 );
}

void SudokuDialog::on_b_67_clicked()
{
    process_click( 6, 7 );
}

void SudokuDialog::on_b_77_clicked()
{
    process_click( 7, 7 );
}

void SudokuDialog::on_b_87_clicked()
{
    process_click( 8, 7 );
}

void SudokuDialog::on_b_08_clicked()
{
    process_click( 0, 8 );
}

void SudokuDialog::on_b_18_clicked()
{
    process_click( 1, 8 );
}

void SudokuDialog::on_b_28_clicked()
{
    process_click( 2, 8 );
}

void SudokuDialog::on_b_38_clicked()
{
    process_click( 3, 8 );
}

void SudokuDialog::on_b_48_clicked()
{
    process_click( 4, 8 );
}

void SudokuDialog::on_b_58_clicked()
{
    process_click( 5, 8 );
}

void SudokuDialog::on_b_68_clicked()
{
    process_click( 6, 8 );
}

void SudokuDialog::on_b_78_clicked()
{
    process_click( 7, 8 );
}

void SudokuDialog::on_b_88_clicked()
{
    process_click( 8, 8 );
}

void SudokuDialog::update()
{
    ui.numSolutions->setText( QString( "%1" ).arg( num_solutions ) );
    // Sync the field.
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            set_button( bt[x][y], field.field[x][y], solved_field.field[x][y],
                num_solutions );
        }
    }
    // Update status
    if( solver.isRunning() )
    {
        ui.label_Status->setText( tr( "Идет расчет" ) );
        ui.label_Status->setStyleSheet( "QLabel { color: red; "
            "background: yellow }" );
    }
    else
    {
        ui.label_Status->setText( tr( "Готово" ) );
        ui.label_Status->setStyleSheet( QString() );
    }
}

