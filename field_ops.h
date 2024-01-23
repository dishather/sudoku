// Sudoku field operations.

#ifndef SUDOKU_FIELD_OPS_H_INCLUDED
#define SUDOKU_FIELD_OPS_H_INCLUDED

struct sudoku_field_t;

// Updates 'allowed' part of the field.
// Returns true if the field is valid, false if the sudoku rules are broken.
bool check_field_allowed( sudoku_field_t &f );

// Checks if the field is valid.
// Only used once after loading the field.
bool is_field_valid( sudoku_field_t const &f );

// if field is NULL, the callback is called as a heartbeat (no result).
typedef bool (* sudoku_callback)(sudoku_field_t *f, void *);

// Solves the puzzle and returns the number of solutions printed.
int solve( sudoku_field_t &f, sudoku_callback cb, void *cookie );

#endif /* SUDOKU_FIELD_OPS_H_INCLUDED */