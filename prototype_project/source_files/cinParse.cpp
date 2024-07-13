#include "../header_files/cinParse.h"
using namespace std;

void cinParse::parse()
{
  // clear all flags:
  _isMoveCommand = false; // indicates whether the command is a move command

  string input;
  getline(cin, input); // retrieve using standard intput

  if (input.length()) // make sure input not empty
  {
    // set all chars to lowercase:
    int charCount;
    for (charCount = 0; charCount < input.length(); charCount++)
    {
      input[charCount] = tolower(input[charCount]);
    }
    {
      ////// parse move command //////
      {
        /*
          alphabet and number coordinate can be in either order,
          check both.
          x coord = alphabetical
          y coord = numerical
        */
        string move_string = input;
        if (move_string[0] >= 'a' && move_string[0] <= 'z') // alphabetical first coordinate
        {
          // set x coord
          _moveCommand.coord_x = move_string[0] - 'a';
          move_string = move_string.substr(1); // remove alphanumerical char

          if (isdigit(move_string[0])) // validate next char is digit
          {
            try
            {
              // set y coord
              _moveCommand.coord_y = stoi(move_string);

              // Remove digits from the string until a non-digit character is found
              move_string = move_string.substr(1);
              while (move_string.length() && isdigit(move_string[0]))
              {
                move_string = move_string.substr(1); // remove one character
              }

              /*
                two coordinates have been found,
                one of two options left now.
                an action command(d = dig, f = flag)
                or a default action command(dig)
              */
              if (move_string.length() == 1)
              {
                // check whihc action command selected:
                switch (move_string[0])
                {
                case 'd':
                  _moveCommand.commandAction = false;
                  _isMoveCommand = true; // set flag
                  break;
                case 'f':
                  _moveCommand.commandAction = true;
                  _isMoveCommand = true; // set flag
                  break;
                }
              }
              else if (!move_string.length())
              {
                // set default action command
                _moveCommand.commandAction = false;
                _isMoveCommand = true; // set flag
              }
            }
            catch (exception &err)
            {
              // failed to convert to int, simply ignore
            }
          }
        }
        else // numerical first coordinate
        {
          if (isdigit(move_string[0]))
          {
            try
            {
              // set y coord
              _moveCommand.coord_y = stoi(move_string);

              // Remove digits from the string until a non-digit character is found
              move_string = move_string.substr(1);
              while (move_string.length() && isdigit(move_string[0]))
              {
                move_string = move_string.substr(1); // remove one character
              }

              if (move_string[0] >= 'a' && move_string[0] <= 'z')
              {
                // set x coord
                _moveCommand.coord_x = move_string[0] - 'a';
                move_string = move_string.substr(1); // remove alphanumerical char

                /*
                  two coordinates have been found,
                  one of two options left now.
                  an action command(d = dig, f = flag)
                  or a default action command(dig)

                */
                if (move_string.length() == 1)
                {
                  // check whihc action command selected:
                  switch (move_string[0])
                  {
                  case 'd':
                    _moveCommand.commandAction = false;
                    _isMoveCommand = true; // set flag
                    break;
                  case 'f':
                    _moveCommand.commandAction = true;
                    _isMoveCommand = true; // set flag
                    break;
                  }
                }
                else if (!move_string.length())
                {
                  // set default action command
                  _moveCommand.commandAction = false;
                  _isMoveCommand = true; // set flag
                }
              }
            }
            catch (exception &err)
            {
              // failed to convert to int, simply ignore
            }
          }
        }
      }
    }
  }
}

bool cinParse::isMoveCommand()
{
  return _isMoveCommand;
}

cinParse::move_struct cinParse::getMoveCommand()
{
  // check if there is a valid move command:
  if (_isMoveCommand)
  {
    return _moveCommand;
  }

  // return default values:
  move_struct temp = {-1, -1, false};
  return temp;
}