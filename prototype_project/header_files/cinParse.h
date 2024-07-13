#ifndef cinParse_h
#define cinParse_h

/*
  amir gorkovchenko
  
  this class will parse for game move commands
  it is only a prototype class to test the actual game engine
*/

#include <iostream>
#include <string>
#include <stdint.h> // << for 'uint8_t'
using namespace std;

class cinParse
{
public:
  // structs to help organize data:
  struct size_struct
  {
    int coord_x = -1;
    int coord_y = -1;
  };
  struct move_struct
  {
    int coord_x = -1;
    int coord_y = -1;

    bool commandAction = false; // false = dig, true = flag
  };

  /*
       input is read from standard input (cin) using getline
       the input is assumed to be a string representing commands and parameters

       - this is a blocking function, it will wait until any character is entered via cin
       - inputs are converted to lowercase
       - the move command is parsed and if valid, sets a flag that can be retrievd using getter commands

      - this class simply parses input, it will not validate data or print error messages
   */
  void parse();

  /*
     returns: (boolean) is move command flag from previous parse
   */
  bool isMoveCommand();


  /*
    checks if isMoveCommand is set, return default values otherwise
    returns: (move_struct) structure with size in x and y coordinates and action command
  */
  move_struct getMoveCommand();

private:
  // structure to hold start size and move command values:
  size_struct _startSize;
  move_struct _moveCommand;

  // flags to be set during parsing:
  bool _isMoveCommand = false;
};

#endif // #ifndef cinParse_h