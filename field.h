// Sudoku field

#ifndef SUDOKU_FIELD_H_INCLUDED
#define SUDOKU_FIELD_H_INCLUDED

#include <set>
#include <iosfwd>

struct sudoku_field_t {
    typedef std::set<unsigned char> set_t;

    unsigned char field  [9][9];
    set_t         allowed[9][9];
};

#endif /* SUDOKU_FIELD_H_INCLUDED */