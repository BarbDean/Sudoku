//
// Written by:  Barbara Dean
//              President
//              High Mountain Technologies
//              Breckenridge CO
//
// main.cpp, sudoku.cpp/h, digit.cpp/h
//

#include <iostream>
#include "sudoku.h"

using namespace std;

int main( int argc, char **argv )
{
   try {
      string infile = "input.txt";
      if ( argc > 1 )
      {
         infile = argv[1];
      }
    
      sudoku game ( infile );
      game.solve();
   }
   catch (int e) {
      cout << "Exception: " << e << endl;
   }
}
