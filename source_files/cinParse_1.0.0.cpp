#include "../header_files/cinParse_1.0.0.h"

void cinParse::parse()
{
  // delete previous tokens:
  _tokenList.clear();

  ////// push input as individual words into list //////
  {
    // wait until user input:
    string input;
    getline(cin, input);

    string word; // to split into individual words/tokens

    for (char c : input)
    {
      if (isspace(c)) // this includes \t
      {
        if (!word.empty()) // push into list only if something available
        {
          _tokenList.push_back(word);
          word.clear(); // prepare for next word
        }
      }
      else
      {
        word += tolower(c); // only lowercase wanted for consistency
      }
    }

    // last word also just in case
    if (!word.empty())
      _tokenList.push_back(word);
  }
}

uint16_t cinParse::tokenCount()
{
  return _tokenList.size();
}

cinParse::token cinParse::getToken(uint16_t index)
{
  /*
    parse target token into easy to use data
  */

  if (index >= tokenCount())
    return {"", -1, {-1, -1}}; // invaliid index

  token parse = {"", -1, {-1, -1}}; // return value
  parse.s = _tokenList[index];

  ////// parse single integer //////
  // convert to strict integer if possible. (only positive integers allowed, no stray chars)
  {
    bool allDigits = true;
    for (char c : parse.s)
    {
      if (!isdigit(c))
      {
        allDigits = false;
        break;
      }
    }

    if (allDigits)
    {
      try
      {
        parse.value = stoi(parse.s);
      }
      catch (exception &err)
      {
        parse.value = -1;
      }
    }
    else
      parse.value = -1;
  }

  ////// parse compound integers's //////
  /*
    convert to compound integer if possible
    alphabet and number coordinate can be in either order, eg. f4 or 4f
      check both.
      alphabetical (0 - 25)
      numerical  (0 - INT_MAX)
  */
  {
    string compoundString = parse.s;
    if (compoundString.length() > 1) // must have at least two chars to begin with
    {
      if (compoundString[0] >= 'a' && compoundString[0] <= 'z') // << alphabetical first coordinate
      {
        parse.compound_value[0] = compoundString[0] - 'a';
        compoundString = compoundString.substr(1); // remove char

        bool allDigits = true;
        for (char c : compoundString)
        {
          if (!isdigit(c))
          {
            allDigits = false;
            break;
          }
        }

        if (allDigits)
        {
          // attempt to convert to integer:
          try
          {
            parse.compound_value[1] = stoi(compoundString);
          }
          catch (exception &err)
          {
            parse.compound_value[1] = -1;
          }
        }
        else
          parse.compound_value[1] = -1;
      }
      else if (isdigit(compoundString[0])) // << numerical value first
      {
        // attempt to convert to integer:
        try
        {
          parse.compound_value[1] = stoi(compoundString);
        }
        catch (exception &err)
        {
          parse.compound_value[1] = -1;
        }

        // remove all digit chars:
        compoundString = compoundString.substr(1);
        while (compoundString.length() && isdigit(compoundString[0]))
          compoundString = compoundString.substr(1);

        if (compoundString.length() == 1)
        {
          if (compoundString[0] >= 'a' && compoundString[0] <= 'z')
          {
            parse.compound_value[0] = compoundString[0] - 'a';
          }
        }
      }
    }

    /*
      if one or the other compound conversion failed, make sure both reflect that:
    */
    if (parse.compound_value[0] < 0 || parse.compound_value[1] < 0)
    {
      parse.compound_value[0] = -1;
      parse.compound_value[1] = -1;
    }
  }

  // return converted data:
  return parse;
}