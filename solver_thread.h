// Solver thread.

#include <QThread>

struct sudoku_field_t;

class SolverThread: public QThread
{
public:
    SolverThread( sudoku_field_t &in, sudoku_field_t &out, int &solutions );
    void run();
    void set_fresh() { is_fresh = true; break_ = true; }

private:
    sudoku_field_t &in_  ;
    sudoku_field_t &out_ ;
    int            &solutions_;
    volatile bool   is_fresh;
    volatile bool   break_;
};
