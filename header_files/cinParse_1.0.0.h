/*
  Amir Gorkovchenko - CSC1061X41 Capstone

  this class is a base class that helps for parsing cin inputs
  it will take any input given and 'tokenize' them, making them easier to parse for child classes
  each token from an input will also be converted to a set of useful values such as a compound value and a single value.
*/

#ifndef cinParse_h
#define cinParse_h

#include <iostream>
#include <string>
#include <stdint.h> // << for 'uint8_t'
#include <vector>

using namespace std;

class cinParse
{
public:
  /*
    since both child classes happen to use this, it will be implemented here
     output: (boolean) is character command flag from previous parse
  */
  bool isCharCommand()
  {
    return _isCharCommand;
  }

  /*
    check if isCharCommand is true, return error otherwise
    output: (int) char command flag from previous parse
      -1 = error / no char command
      0 = quit command
      1 = start game command
      2 = start game command with size
      4 = help command
  */
  int getCharCommand()
  {
    if (_isCharCommand)
      return _charCommand;
    return -1;
  }

protected:
  /*
    when retrieving tokens,
    they may need to be converted into integers
    compound value will be for 'a2' g54' type commands with two separate commands
    alphabetic being first
  */
  struct token
  {
    string s;
    int value;
    int compound_value[2]; // << alphabetic char [0], numerical value [1]
  };

  /*
    waits for an input, parses entire line
    separates by whitespaces or other separating characters

    resets internal vector each time called
    getter functions can be used to extract data
  */
  void parse();

  /*
    get number of tokens available
  */
  uint16_t tokenCount();

  /*
    get a targetted token at given index
  */
  token getToken(uint16_t index);

  bool _isCharCommand = false;
  /*
    _charCommand:
     -1 = error / no char command
      0 = quit command
      1 = start game command
      2 = start game command with size
      4 = help command
  */
  int _charCommand = -1;

private:
  vector<string> _tokenList;
};

#endif // #ifndef cinParse_h