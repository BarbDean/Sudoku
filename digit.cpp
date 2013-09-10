
//
// Written by:  Barbara Dean
//              High Mountain Technologies
//              Breckenridge CO
//

#include <cstdlib>
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

