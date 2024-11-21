#include "../header_files/menuParse_1.0.0.h"

void menuParse::parse()
{
  // parent parser will tokenize our input:
  cinParse::parse();

  // init flags:
  _isStartGameCommand = false;
  _isCharCommand = false;

  if (tokenCount())
  {
    token tok = getToken(0);

    /*
      single char commands:
    */
    if (tokenCount() == 1)
    {
      if (tok.s.length() == 1) // one char token
      {
        switch (tok.s[0])
        {
        case 'x':
          // quit command
          _charCommand = 0;
          _isCharCommand = true;
          break;
        case '?':
          // help command
          _charCommand = 4;
          _isCharCommand = true;
          break;
        case 's':
          // start default command
          _charCommand = 1;
          _isCharCommand = true;
          break;
        }
      }
    }
    else if (tokenCount() == 3) /* start command */
    {
      if (tok.s.length() == 1 && tok.s[0] == 's') // validate first char
      {
        _startGameCommand.x = getToken(1).value;
        _startGameCommand.y = getToken(2).value;

        if (_startGameCommand.x > -1 && _startGameCommand.y > -1) // validate that values were parsed
        {
          // help command
          _charCommand = 2; // << this is weird setting it as a single char command, but it makes it easier
          _isCharCommand = true;
          _isStartGameCommand = true;
        }
      }
    }
  }
}

bool menuParse::isStartGameCommand()
{
  return _isStartGameCommand;
}

menuParse::startGame_struct menuParse::getStartGameCommand()
{
  if (_isStartGameCommand)
    return _startGameCommand;

  // return default values;
  return {-1, -1};
}