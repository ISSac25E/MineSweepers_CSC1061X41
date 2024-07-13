#include <iostream>
#include <string>
#include "header_files/mineSweeper.h"
#include "header_files/cinParse.h"
using namespace std;

/*
  tested:
    g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
*/

/*
  amir gorkovchenko
*/

/*
  A lot that isn't done here, but its a good proof of concept

  because the parsing classes aren't done yet, only the move commands work
  A single minesweeper game is loaded and can be played until game-won or game-over

  This minesweeper game uses a plain vector for each block
*/

void printInstructions();
void printBoard(mineSweeper &);
void printInvalidInput(string error = "");

int main()
{

  mineSweeper game;
  bool run = true;

  printInstructions();

  game.startNewGame(20, 20);

  while (run)
  {

    if (game.getStatus() != 2)
    {
      // TODO
    }
    else
    {

      printBoard(game);
      cout << "Enter Command..." << endl;

      bool validInput = false;
      cinParse parseInput;

      while (!validInput)
      {
        parseInput.parse();
        if (parseInput.isMoveCommand())
        {

          if (parseInput.getMoveCommand().coord_y >= 1 && parseInput.getMoveCommand().coord_y <= game.getSize_y() &&
              parseInput.getMoveCommand().coord_x >= 0 && parseInput.getMoveCommand().coord_x < game.getSize_x())
          {

            if (!parseInput.getMoveCommand().commandAction &&
                game.getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isCovered &&
                game.getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isFlagged)
            {

              printInvalidInput("Block is flagged");
            }
            else
            {

              validInput = true;
              game.move(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1, parseInput.getMoveCommand().commandAction);

              switch (game.getStatus())
              {
              case 0:
                printBoard(game);
                cout << ">>>> Game Over! You Lost <<<<" << endl;
                run = false;
                break;
              case 1:
                printBoard(game);
                cout << ">>>> Congratulations! You Won <<<<" << endl;
                run = false;
                break;
              }
            }
          }
          else
          {

            printInvalidInput(string("Out of bounds. Max(") + (char)(game.getSize_x() + 'a' - 1) + ", " + to_string(game.getSize_y()) + ")");
          }
        }
        else
        {

          printInvalidInput();
        }
      }
    }
  }

  cout << "Exiting..." << endl;
  return 0;
}

/*
  input: none
  process: print simple game instructions
  output: none
 */
void printInstructions()
{
  cout << endl;
  cout << "(NOT IMPLEMENTED! >> ) To start a new game, press 's'. To start at custom size, type 's x y'" << endl;
  cout << "(NOT IMPLEMENTED! >> ) To save and exit anytime, press 'x'" << endl;
  cout << "Game Play:" << endl;
  cout << "  To enter a column, use letters a-h. To enter a row, use numbers 1-8" << endl;
  cout << "  To dig, use 'd' and to flag, use 'f'" << endl;
  cout << "  Type everything into a single command with row-column first then action like this: '1ad' or 'b3f'" << endl;
  cout << "(NOT IMPLEMENTED! >> ) To see these instructions again, press '?' anytime" << endl;
  cout << endl;
}

/*
    determine state of game and each block
    print in easy to read format and react well to scaling(larger x and y axis)

    inputs: (mineSweeper &) game object to be printed
 */
void printBoard(mineSweeper &game)
{

  uint8_t maxBlankSpace = 1;
  {
    int size_y_temp = game.getSize_y();
    while (size_y_temp /= 10)
    {
      maxBlankSpace++;
    }
  }

  int cnt_x;
  int cnt_y;

  {

    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      {
        cout << " ";
      }
    }

    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {
      cout << char('A' + cnt_x) << "  ";
    }
    cout << endl;
  }

  for (cnt_y = 0; cnt_y < game.getSize_y(); cnt_y++)
  {
    /*
      calc digit length for each x axis number:
     */
    uint8_t digitLen = 1;
    int cnt_y_temp = cnt_y + 1;
    while (cnt_y_temp /= 10)
    {
      digitLen++;
    }

    /*
      add spaces before each x axis number:
     */
    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex < maxBlankSpace - digitLen; spaceIndex++)
      {
        cout << " ";
      }
    }

    cout << cnt_y + 1 << ' ';

    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {

      if (game.getStatus() == 0 || game.getStatus() == 1)
      {
        if (game.getBlock(cnt_x, cnt_y).isBomb)
        {
          /*
            print 'O' for a won game
            print 'X' for lost game:
           */
          if (game.getStatus() == 1)
          {
            cout << "O ";
          }
          else
          {
            cout << "X  ";
          }
        }
        else
        {
          cout << "   ";
        }
      }
      else
      {
        if (game.getBlock(cnt_x, cnt_y).isCovered)
        {

          if (game.getBlock(cnt_x, cnt_y).isFlagged)
          {
            cout << "O  ";
          }
          else
          {
            cout << char(45) << "  ";
          }
        }
        else
        {
          /*
            uncovered blocks can be one of 3 types
             bomb
             number hint
             empty
           */
          if (game.getBlock(cnt_x, cnt_y).isBomb)
          {
            cout << "X  ";
          }
          else if (game.getBlock(cnt_x, cnt_y).blockNum > 0)
          {
            cout << to_string(game.getBlock(cnt_x, cnt_y).blockNum) << "  ";
          }
          else
          {
            cout << "   ";
          }
        }
      }
    }
    cout << endl;
  }

  {

    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      {
        cout << " ";
      }
    }

    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {
      cout << char('A' + cnt_x) << "  ";
    }
  }
  cout << endl
       << endl;
}

/*
    print invalid input message, if needed, input clarification message
    inputs: default arg, error string(for error clarification)
 */
void printInvalidInput(string error /*= ""*/)
{
  cout << "Invalid Input! " << error << endl;
  cout << "(NOT IMPLEMENTED! >> ) Press '?' for help or try again" << endl;
}
