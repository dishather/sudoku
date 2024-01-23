// Implements sudoku field operations.

#include "field.h"
#include "field_ops.h"

struct solve_context_t {
    sudoku_callback     cb;
    void               *cookie;
    bool                stop;
    int                 numsol;
};

namespace {

void init_allowed( sudoku_field_t &f )
{
    sudoku_field_t::set_t s;
    for( unsigned char i = '1'; i <= '9'; ++i )
    {
        s.insert( i );
    }
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            if( f.field[x][y] != 0 )
            {
                f.allowed[x][y].clear();
            }
            else
            {
                f.allowed[x][y] = s;
            }
        }
    }
}


bool check_allowed( sudoku_field_t const &f )
{
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            if( f.field[x][y] == 0 )
            {
                if( f.allowed[x][y].empty() )
                {
                    return false;
                }
            }
        }
    }
    return true;
}


bool find_most_obvious_cell( sudoku_field_t const &f, int &ax, int &ay )
{
    size_t smallest = 10; // smallest set found so far.
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            if( ( f.field[x][y] == 0 ) &&
                ( f.allowed[x][y].size() < smallest ) )
            {
                ax = x;
                ay = y;
                smallest = f.allowed[x][y].size();
            }
        }
    }
    return smallest != 10;
}


void solve( sudoku_field_t &f, solve_context_t &ctx )
{
    if(! check_field_allowed( f ) )
    {
        return;
    }

    int x = 0, y = 0;
    // We won't be able to find next cell only if the field is solved.
    if(! find_most_obvious_cell( f, x, y ) )
    {
        ++ctx.numsol;
        ctx.stop |= ctx.cb( &f, ctx.cookie );
        return;
    }
    sudoku_field_t::set_t::const_iterator
        i = f.allowed[x][y].begin(),
        e = f.allowed[x][y].end();
    for( ; i != e; ++i )
    {
        sudoku_field_t f1 = f;
        f1.field[x][y] = *i;

        solve( f1, ctx );

        if( ctx.stop )
        {
            return;
        }
    }
    ctx.stop |= ctx.cb( NULL, ctx.cookie );
}

} // anonymous namespace ends

bool check_field_allowed( sudoku_field_t &f )
{
    init_allowed( f );

    // Remove known numbers from the lists of allowed
    // numbers for all verticals, horizontals, and 3x3 squares.
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            if( f.field[x][y] != 0 )
            {
                unsigned char const c = f.field[x][y];

                // Remove from all verticals and horizontals.
                for( int z = 0; z < 9; ++z )
                {
                    f.allowed[x][z].erase( c );
                    f.allowed[z][y].erase( c );
                }
                // Remove from all 3x3 squares. This is a bit tricky.
                // Get the upper left corner of the square
                int const xs = x - x % 3;
                int const ys = y - y % 3;
                // ... and remove chars
                for( int x1 = 0; x1 < 3; ++x1 )
                {
                    for( int y1 = 0; y1 < 3; ++y1 )
                    {
                        f.allowed[ xs + x1][ ys + y1 ].erase( c );
                    }
                }
            }
        }
    }
    return check_allowed( f );
}


// Solves the puzzle and returns the number of solutions printed.
int solve( sudoku_field_t &f, sudoku_callback cb, void *cookie )
{
    solve_context_t ctx = {
        cb, cookie, false, 0
    };
    solve( f, ctx );
    return ctx.numsol;
}

bool is_field_valid( sudoku_field_t const &f )
{
    // Values must be 0 (empty) or '1' .. '9'.
    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            unsigned char c = f.field[x][y];
            if( ( c != 0 ) && ( c < '1' ) && ( c > '9' ) )
            {
                return false;
            }
        }
    }

    // Horizontals, verticals and 3x3 cells.
    // Each must have no more than 1 figure of a kind.
    sudoku_field_t::set_t horiz[9];
    sudoku_field_t::set_t vert[9];
    sudoku_field_t::set_t cells[3][3];

    for( int y = 0; y < 9; ++y )
    {
        for( int x = 0; x < 9; ++x )
        {
            unsigned char c = f.field[x][y];
            if( c != 0 )
            {
                std::pair<sudoku_field_t::set_t::iterator, bool> res = horiz[x].insert( c );
                if( ! res.second )
                {
                    return false;
                }
                res = vert[y].insert( c );
                if( ! res.second )
                {
                    return false;
                }
                res = cells[x / 3][y / 3].insert( c );
                if( ! res.second )
                {
                    return false;
                }
            }
        }
    }

    return true;
}
