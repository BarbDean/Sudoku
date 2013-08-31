//
// Written by:  Barbara Dean
//              President
//              High Mountain Technologies
//              Breckenridge CO
//
// main.cpp, sudoku.cpp/h, digit.cpp/h
// compiled under cygwin. 
// need a few minor changes to compile under visual c++
//

#include <iostream>
#include "sudoku.h"

using namespace std;

int main( int argc, char **argv )
{
   string infile = "input.txt";
   if ( argc > 1 )
   {
      infile = argv[1];
   }
      
   sudoku game ( infile );
   game.solve();
}

***************************************************

//
// Written by:  Barbara Dean
//              President
//              High Mountain Technologies
//              Breckenridge CO
// 
#include <fstream>
#include <iostream>
#include <set>
#include <math.h>
#include "sudoku.h"
#include "digit.h"

using namespace std;

sudoku::sudoku( const string &infile ): currentIndex(-1)
{
   // Read the input file
   int count = readInput( infile );

   // Set a few local variables for convenience
   boardDim = static_cast<int>( sqrt( count ) );
   totalDigits = boardDim * boardDim;

   // Sanity check the input before proceeding
   errorCheckInput( count );

   // This pre-determines all of the indices that need to
   // be checked for any given digit in the board
   setMaps();

   // Print out the input
   cout << "Input: " << endl;
   printBoard();
}

void sudoku::solve( )
{
  // Outter loop will keep going until a solution is found.
  // ( Gulp! Can you say 'infinite loop'???? Yikes! )
  for ( ; ; )
  {
     // Set currentIndex to the first occurrance of a zero in the board
     if ( findFirstZero( ) )
     {
        // There are no more zeros, which means we have solution
        // Hip-hip-Hooray!
        cout << "Solution:  " << endl;
        printBoard();
        return;
     }
     else
     {
        // This is a flag that indicates whether to move forward
        // or to revisit an old digit
        bool backup = false;

        // Geez -- another 'infinite loop'
        // This time, looping until we find some valid value --
        // either by moving forward, or by backing up as far as
        // necessary
        for ( ; ; )
        {
           bool foundGoodValue = lookForValidValue ( backup  );

           // Found a good value, so pop out of this loop and look
           // for the next zero entry (top of outter loop)
           if ( foundGoodValue )
              break;
           // Couldn't fill in this index with a valid value, so
           // back up and try again.
           else
              backup = true;
        }
     }
  }
}

void sudoku::printBoard( )
{
   for (int i =0; i < totalDigits; i++)
   {
      cout << mBoard[i] << "\t";
      if ( ( (i+1) % (boardDim) == 0  && i != 0 ))
         cout << endl;
   }
   cout << endl << endl;
}

// This function checks the row, column and box for
// the i'th entry on the board
bool sudoku::checkValue( )
{
   int i = currentIndex; // shorthand

   // Set up a map of the row, col and box indices so
   // that we can just loop over the map
   map < int, vector <int> >indices ;
   indices[0] = mRowMap[ i ];
   indices[1] = mColMap[ i ];
   indices[2] = mBoxMap[ i ];

   // Loop over each dimension that needs to be checked
   // (row, col, box, of course...)
   for ( int k = 0 ; k < indices.size() ; k++)
   {
      // Get the indices for the current dimension
      vector < int > indicesToCheck = indices[ k ];
      set < int > uniqueNums;
      int count = 0;

      // Loop over the indices for this dimension
      for ( int j = 0 ; j < indicesToCheck.size(); j++ )
      {
         // Get the value of the digit from the board for
         // this index
         int digit = mBoard[ indicesToCheck[ j ] ];

         // Ignore zeros
         if ( digit != 0 )
         {
            // Insert the digit into a set.  By definition, the set
            // will contain only 1 instance of a given value, thus
            // we can determine whether there were redundant values
            // by checking the size of the set against the number of
            // insertions.
            uniqueNums.insert( digit );
            count++;
         }
      }

      // If the count is not the same as the number of
      // unique numbers in the set, then there were duplicates
      // which is illegal...
      bool returnVal = true;
      if ( uniqueNums.size() != count )
      {
         return false;
      }
    }

    // If it made it this far, all of the values in the row, col
    // and box are valid
    return true;
}

//  This function sets up maps of input board index
//  to vectors of indices that make up the row, column
//  and box constraints.
void sudoku::setMaps ( )
{
   // Loop over all of the digits so that we generate a map
   // setting for each digit on the board
   for ( int j=0; j < totalDigits ; j++)
   {
      int rowNumber = j / boardDim;
      int colNumber = j % boardDim;
      vector <int> rowIndices;
      vector <int> colIndices;
      for ( int i = 0 ; i < boardDim; i++ )
      {
         rowIndices.push_back( rowNumber * boardDim + i );
         colIndices.push_back( colNumber + boardDim * i);
      }
      mRowMap[ j ] = rowIndices;
      mColMap[ j ] = colIndices;

      int boxDim  = static_cast<int>( sqrt( boardDim ));
      int boxPosX = rowNumber /  boxDim ;
      int boxPosY = colNumber /  boxDim ;
      vector <int> boxIndices;
      for ( int kk = 0; kk < boxDim; kk++)
      {
         for ( int yy = 0; yy < boxDim; yy++ )
         {
            boxIndices.push_back ( 
               ( boxPosX * boxDim + kk )  * boardDim  // This gets to
                                                      // correct row
             +   boxPosY * boxDim + yy ) ;            // This gets to
                                                      // correct column
          }
      }
      mBoxMap[ j ] = boxIndices;
   }
}

// This function looks for the first occurrance of a zero
// on the board.  If there are no zeros, we have a solution
bool sudoku::findFirstZero( )
{
   bool foundSolution = true;
   int i = 0;
   for ( i = 0; i < mBoard.size() ; i++)
   {
      if ( mBoard[i] == 0 )
      {
           foundSolution = false;
           break;
       }
    }

    // 'currentIndex' is owned by the sudoku class and is
    // used in several functions.  It is set here, but could be
    // re-set in lookForValidValue
    currentIndex = i;
    return foundSolution;
}

// This function attempts to find a valid value for the
// position in the board currentIndex
bool sudoku::lookForValidValue( bool backup )
{
   // The 'x' pointer will either get initialized with
   // a new object or it will be set equal to an old
   // object
   digit *x;

   // If "backup" is true, we want to go back to
   // an old digit and modify its value
   if ( backup )
   {
      // Revert back to an old object
      currentIndex =  indicesVector[ indicesVector.size() - 1 ];
      x = digitsVector[ digitsVector.size() - 1 ];

      // Pop off the old object since we will now be modifying it
      indicesVector.pop_back();
      digitsVector.pop_back();
   }
   // Otherwise, move forward and create a new digit
   else
   {
       x = new digit( boardDim );
   }
  
   // Loop over all of the possible values
   for ( int i = 0; i < x->numPossibleValues ; i++)
   {
      // Get the next possible value that could be filled into this slot
      mBoard[ currentIndex ] = x->getNextPossibleValue();
      
      // The value returned by getNextPossibleValue will be
      // -1 when all possible values have been exhausted
      if (mBoard[ currentIndex ] == -1 )
          break;
  
      // Check to see if the number that we just found is valid
      bool foundValidValue = checkValue( );

      // If we have found a valid value for this position in
      // the table, save the digit object
      if ( foundValidValue )
      {
         digitsVector.push_back( x );
         indicesVector.push_back( currentIndex );
         return true;
      }
   }
   // If we have gotton to this point, then we could not
   // find a valid value for this position in the table.
   // Re-set to zero and return
   mBoard[ currentIndex ] = 0;
   return false;
}

sudoku::~sudoku( )
{
   // Delete the digit pointers
   for (int i=0 ; i < digitsVector.size(); i++)
      delete digitsVector[i];
}

// This function will exit if there is a problem with
// the input data
void sudoku::errorCheckInput( int count )
{
   if( totalDigits != count )
   {
      cout << "Error in input file: the number of entries in"
           << " the input file must be a perfect square. " << endl 
           << "There were " << count << " entries in the input file " << endl;
      exit( 0 );
   }
   for (int i = 0; i < mBoard.size() ; i++ )
   {
      if (mBoard[i] < 0 || mBoard[i] > boardDim )
      {
         cout << "Input data out of range " << mBoard[i] << endl;
         exit( 0 );
       }
   }
}

int sudoku::readInput( const string &infile )
{
   ifstream in;
   in.open( infile.c_str() );

   if( !in.is_open() )
   {
      cout << "Could not open the input file " << infile << endl;
      exit(0);
   }

   int tmp = 0;
   int count = 0;
   for ( ;; count++ )
   {
      in >> tmp;
      if ( in.eof() )
         break;
      else
          mBoard.push_back(tmp);
   }
   in.close();
   return count;
}

***************************************************

//
// Written by:  Barbara Dean
//              President
//              High Mountain Technologies
//              Breckenridge CO
//

#include <iostream>
#include <set>
#include "digit.h"

using namespace std;

digit::digit( int boardDim ): index(0), numPossibleValues( boardDim )
{
   // Resize the Digits Vector to the Board Dimension
   possibleValues.resize(boardDim);

   // Fill the vector from 1..boardDim
   for (int i = 1; i <= boardDim; i++)
   {
      possibleValues[i -1] = i;
   }

   // Randomly Shuffle the order of the possibleValues
   srand(unsigned(time(0)));
   for (int i = 0; i < boardDim; i++)
   {
      int randNum1 = rand() % boardDim;
      int randNum2 = rand() % boardDim;
      int tmp = possibleValues[randNum1];
      possibleValues[randNum1] = possibleValues[randNum2];
      possibleValues[randNum2] = tmp;
   }
}

int digit::getNextPossibleValue( )
{
   // The return value defaults to -1, which is what will
   // be returned after all possibilities have been exhausted
   // for this digit
   int returnVal = -1;
   if( index < numPossibleValues ) 
   {
      returnVal =  possibleValues[ index ] ;
      index++;
   }
   return returnVal;
}

***************************************************

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

     // Sanity check the input before trying to solve the puzzle
     void errorCheckInput( int count );
   
};

#endif

***************************************************

#ifndef DIGIT_H
#define DIGIT_H

#include <vector>

class digit
{
  public:
     digit( int boardDim );
     ~digit(){ } ;

     // Vector of possible values.  Will be of
     // length 'boardDim' and will contain the values
     // 1..boardDim in a random order
     std::vector <int> possibleValues;

     // Index into the possibleValues vector
     int index;

     // Return the next possible Value
     int getNextPossibleValue( );

     // Local variable for convenience.
     int numPossibleValues;
};

#endif
