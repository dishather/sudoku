// Solver thread.

#include "solver_thread.h"
#include "field.h"
#include "field_ops.h"

struct cb_context_t {
    sudoku_field_t *f;
    int            *count;
    volatile bool  *stop;
};

namespace {

//
bool stop_after_tenth( sudoku_field_t *solved_field, void *cookie )
{
    cb_context_t *cbc = static_cast<cb_context_t*>( cookie );
    // heartbeat?
    if( ! solved_field )
    {
        return *cbc->stop;
    }
    if( *cbc->count == 0 )
    {
        *( cbc->f ) = *solved_field;
    }
    return *cbc->stop || ( ++(*cbc->count) > 999999 );
}

} // anonymous namespace ends

//
SolverThread::SolverThread( sudoku_field_t &in, sudoku_field_t &out, int &solutions ):
    in_  ( in ),
    out_ ( out ),
    solutions_ ( solutions ),
    is_fresh( false ),
    break_ ( false )
{}

//
void SolverThread::run()
{
    while( is_fresh )
    {
        solutions_ = 0;
        cb_context_t cbc = {
                &out_,
                &solutions_,
                &break_
            };
        is_fresh = false;
        break_   = false;
        solve( in_, stop_after_tenth, &cbc );
    }
}

