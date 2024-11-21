#include "../header_files/gameParse_1.0.0.h"

void gameParse::parse()
{
  // parent parser will tokenize our input:
  cinParse::parse();

  // init flags:
  _isMoveCommand = false;
  _isCharCommand = false;

  // only a valid input if tokens exist:
  if (tokenCount())
  {
    token tok = getToken(0);

    /*
      different game commands use different number of tokens
      1 token = exit or help command
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
        }
      }

      /*
        compound move commands are considered as one token
        one valid value means both are acquired (alphabetic and numerical)
      */
      if (tok.compound_value[0] > -1)
      {
        _moveCommand.coord_x = tok.compound_value[0]; // alphabetic char x-coord
        _moveCommand.coord_y = tok.compound_value[1]; // numerical char y-coord

        _moveCommand.commandAction = false;
        _isMoveCommand = true;
      }
    }
    else if (tokenCount() == 2) // this can only be one type of command
    {
      if (tok.compound_value[0] > -1)
      {
        /*
          retrieve compound move AND action command:
        */
        _moveCommand.coord_x = tok.compound_value[0];
        _moveCommand.coord_y = tok.compound_value[1];

        /*
          get second token to parse
        */
        tok = getToken(1);
        if (tok.s.length() == 1)
        {
          switch (tok.s[0])
          {
          case 'd':
            /* dig command */
            _moveCommand.commandAction = false;
            _isMoveCommand = true; // mark as valid command
            break;
          case 'f':
            /* dig flag */
            _moveCommand.commandAction = true;
            _isMoveCommand = true; // mark as valid command
            break;
          }
        }
      }
    }
  }
}

bool gameParse::isMoveCommand()
{
  return _isMoveCommand;
}

gameParse::move_struct gameParse::getMoveCommand()
{
  // check if there is a valid move command:
  if (_isMoveCommand)
    return _moveCommand;

  // return default values:
  return {-1, -1, false};
}