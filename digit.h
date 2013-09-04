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
