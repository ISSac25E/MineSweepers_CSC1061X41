/*
  Amir Gorkovchenko - CSC1061X41 Capstone

  this class inherits from cinParse class and expands on its functionality specifically for in-game commands
  along side from simple single char commands, it will also parse a game-move command that take a compound value token
*/
#ifndef gameParse_h
#define gameParse_h

#include <string>
#include <stdint.h> // << for 'uint8_t'
#include "cinParse_1.0.0.h"

class gameParse : public cinParse
{
public:
  struct move_struct
  {
    int coord_x = -1;
    int coord_y = -1;

    bool commandAction = false; // false = dig, true = flag
  };

  /*
    parse line and set internal flags
    use getter methods to access commands

    this will use cinParse::parse() with some added filtering for game parsing
  */
  void parse();

  /*
     output: (boolean) is move command flag from previous parse
   */
  bool isMoveCommand();

  /*
    check if isMoveCommand is set, return default values otherwise
    output: (move_struct) structure with size in x and y coordinates and action command
  */
  move_struct getMoveCommand();

private:
  move_struct _moveCommand;

  // flags to be set during parsing:
  bool _isMoveCommand = false;
};

#endif // #ifndef gameParse_h