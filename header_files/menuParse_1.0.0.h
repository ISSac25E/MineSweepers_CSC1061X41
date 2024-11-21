/*
  Amir Gorkovchenko - CSC1061X41 Capstone

  this class inherits from cinParse class and expands on its functionality specifically for a game menu
  along side from simple single char commands, it will also parse a 'start custom game' command. This consists of three separate tokens
*/

#ifndef menuParse_h
#define menuParse_h

#include <string>
#include <stdint.h> // << for 'uint8_t'
#include "cinParse_1.0.0.h"

class menuParse : public cinParse
{
public:
  struct startGame_struct
  {
    int x = -1;
    int y = -1;
  };

  /*
    parse line and set internal flags
    use getter methods to access commands

    this will use cinParse::parse() with some added filtering for menu parsing
  */
  void parse();

  /*
     output: (boolean) is start game command flag from previous parse
  */
  bool isStartGameCommand();

  /*
    checks if start game command was set and returns stored values, returns default values otherwise
  */
  startGame_struct getStartGameCommand();

private:
  startGame_struct _startGameCommand;

  // flags to be set during parsing:
  bool _isStartGameCommand = false;
};

#endif