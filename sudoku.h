#include <QDialog>
#include "ui_sudoku.h"
#include "field.h"
#include "solver_thread.h"

class QTimer;

class SudokuDialog : public QMainWindow
{
    Q_OBJECT

public:
    SudokuDialog();
    ~SudokuDialog();

private slots:
    void update();
    void on_b_00_clicked();
    void on_b_10_clicked();
    void on_b_20_clicked();
    void on_b_30_clicked();
    void on_b_40_clicked();
    void on_b_50_clicked();
    void on_b_60_clicked();
    void on_b_70_clicked();
    void on_b_80_clicked();
    void on_b_01_clicked();
    void on_b_11_clicked();
    void on_b_21_clicked();
    void on_b_31_clicked();
    void on_b_41_clicked();
    void on_b_51_clicked();
    void on_b_61_clicked();
    void on_b_71_clicked();
    void on_b_81_clicked();
    void on_b_02_clicked();
    void on_b_12_clicked();
    void on_b_22_clicked();
    void on_b_32_clicked();
    void on_b_42_clicked();
    void on_b_52_clicked();
    void on_b_62_clicked();
    void on_b_72_clicked();
    void on_b_82_clicked();
    void on_b_03_clicked();
    void on_b_13_clicked();
    void on_b_23_clicked();
    void on_b_33_clicked();
    void on_b_43_clicked();
    void on_b_53_clicked();
    void on_b_63_clicked();
    void on_b_73_clicked();
    void on_b_83_clicked();
    void on_b_04_clicked();
    void on_b_14_clicked();
    void on_b_24_clicked();
    void on_b_34_clicked();
    void on_b_44_clicked();
    void on_b_54_clicked();
    void on_b_64_clicked();
    void on_b_74_clicked();
    void on_b_84_clicked();
    void on_b_05_clicked();
    void on_b_15_clicked();
    void on_b_25_clicked();
    void on_b_35_clicked();
    void on_b_45_clicked();
    void on_b_55_clicked();
    void on_b_65_clicked();
    void on_b_75_clicked();
    void on_b_85_clicked();
    void on_b_06_clicked();
    void on_b_16_clicked();
    void on_b_26_clicked();
    void on_b_36_clicked();
    void on_b_46_clicked();
    void on_b_56_clicked();
    void on_b_66_clicked();
    void on_b_76_clicked();
    void on_b_86_clicked();
    void on_b_07_clicked();
    void on_b_17_clicked();
    void on_b_27_clicked();
    void on_b_37_clicked();
    void on_b_47_clicked();
    void on_b_57_clicked();
    void on_b_67_clicked();
    void on_b_77_clicked();
    void on_b_87_clicked();
    void on_b_08_clicked();
    void on_b_18_clicked();
    void on_b_28_clicked();
    void on_b_38_clicked();
    void on_b_48_clicked();
    void on_b_58_clicked();
    void on_b_68_clicked();
    void on_b_78_clicked();
    void on_b_88_clicked();
    
private:
    Ui::Sudoku      ui;
    sudoku_field_t  field;
    sudoku_field_t  solved_field;
    int             num_solutions;
    SolverThread    solver;
    QToolButton    *bt[9][9];
    QTimer         *timer;

    void process_click( int, int );
};
 