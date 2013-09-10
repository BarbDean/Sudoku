
#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <map>
#include "digit.h"

class sudoku
{
  public:
     sudoku ( const std::string &infile );
     ~sudoku( );

     // Toplevel method which is called to solve the puzzle
     void solve( );

     // Sudoku board, flattened out into a vector
     // (flattened in row-major order)
     std::vector <int> mBoard;

     // 4, 9, 16, etc.
     int boardDim;

     // 16, 81, 256, etc.
     int totalDigits;

     // Check whether or not a value meets row, col and
     // box constraints
     bool checkValue( );
     void printBoard( );

     // Setup maps of indices that will be used to check
     // the puzzle constraints
     void setMaps ();

     // Find the first occurrance of a zero on the board
     bool findFirstZero();

     // Vector of digit objects.  The digit objects keep
     // track of valid entries for given spots on the board
     std::vector < digit * > digitsVector;

     // This vector keeps track of the spots on the board
     // that go with the digit objects
     std::vector < int > indicesVector;

     // Board index for the spot on the board which is
     // currently under consideration
     int currentIndex;
     bool lookForValidValue( bool backup );

     // These are maps of indices to vectors of indices
     // The vector of indices tells us which of the
     // totalDigits is used in a given row, column or box.

     // The key (which is the single integer) is a number
     // between 1 and totalDigits.  The length of the vector
     // is boardDim
     std::map < int, std::vector <int> > mRowMap;
     std::map < int, std::vector <int> > mColMap;
     std::map < int, std::vector <int> > mBoxMap;

     // Read the input from file
     int readInput( const std::string &infile );

     // Sanity check the format of the input before trying to solve the puzzle
     bool checkInputFormat( int count );

     // Sanity check the content of the input puzzle
     bool checkInputContent( int totalDigits );

};

#endif

