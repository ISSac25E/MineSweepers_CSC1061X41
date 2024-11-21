/*
  Amir Gorkovchenko - CSC1061X41 Capstone

  tested:
    g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

  mine sweeper program

  plays minesweeper through command line.
  Formats game board using simple characters for an easy to read format
*/
#include <iostream>
#include <string>

// each source file must be compiled separately!
#include "header_files/linkedList_1.1.0.h"
#include "header_files/mineSweeper_1.0.0.h"
#include "header_files/gameParse_1.0.0.h"
#include "header_files/menuParse_1.0.0.h"

using namespace std;

// function prototypes:
void printInstructions();
void printBoard(mineSweeper &);
void printInvalidInput(string error = "");

int main()
{
  mineSweeper *game = nullptr;
  bool run = true;

  printInstructions();

  while (run)
  {
    if (!game || game->gameStatus() != 2) // game not playing, game menu:
    {
      // prompt continuously until valid input provided:
      bool validInput = false; // << dont want to use break in a larger loop sequence
      menuParse parseInput;

      while (!validInput)
      {
        /*
          blocking function
          will wait until any input given
        */
        parseInput.parse();

        switch (parseInput.getCharCommand())
        {
        case 1: /* 's' start default game */
        {
          /*
            setup board and print accordingly:
          */
          const uint8_t default_x = 10;
          const uint8_t default_y = 10;

          // deallocate old game:
          if (game)
            delete game;

          game = new mineSweeper(default_x, default_y);
          cout << "starting new game (" << to_string(default_x) << " x " << to_string(default_y) << ")" << endl;
          validInput = true;
        }
        break;
        case 2: /* "s x y" (start custom game) */
        {
          /*
            validate input size is within range
              x can go from 1-26 (26 letters in alphabet)
              y can go from 1-255 (uint8_t max)
          */
          if (parseInput.getStartGameCommand().x >= 1 && parseInput.getStartGameCommand().x <= 26 &&
              parseInput.getStartGameCommand().y >= 1 && parseInput.getStartGameCommand().y <= 255)
          {
            /*
              setup board and print accordingly:
            */

            // deallocate old game:
            if (game)
              delete game;

            game = new mineSweeper(parseInput.getStartGameCommand().x, parseInput.getStartGameCommand().y);
            cout << "starting new game (" << to_string(parseInput.getStartGameCommand().x) << " x " << to_string(parseInput.getStartGameCommand().y) << ")" << endl;
            validInput = true;
          }
          else
          {
            printInvalidInput("Out of bounds. Max(26, 255)");
          }
        }
        break;
        case 4: /* 'h' (help command) */
        {
          printInstructions();
        }
        break;
        case 0: /* 'x' (quit program) - (AG)*/
        {
          // exit program
          run = false;
          validInput = true;
        }
        break;
        default:
        {
          // unrecognized command in this context
          printInvalidInput();
        }
        break;
        }
      }
    }
    else // game is currently playing:
    {
      // print and prompt for a game move
      printBoard(*game);
      cout << "Enter Command..." << endl;

      // prompt continuously until valid input provided:
      bool validInput = false;
      gameParse parseInput;

      while (!validInput)
      {
        parseInput.parse();

        switch (parseInput.getCharCommand())
        {
        case 4: /* 'h' (help command) */
        {
          printInstructions();
        }
        break;
        case 0: /* 'x' (quit program) - (AG)*/
        {
          // exit game session
          delete game;
          game = nullptr;

          cout << "Quitting Game" << endl;
          printInstructions();
          validInput = true;
        }
        break;
        default:
        {
          // no recognized single char command, check for game move:
          if (parseInput.isMoveCommand())
          {
            /* Verify inputted commands are within range: */
            if (parseInput.getMoveCommand().coord_y >= 1 && parseInput.getMoveCommand().coord_y <= game->getSize_y() &&
                parseInput.getMoveCommand().coord_x >= 0 && parseInput.getMoveCommand().coord_x < game->getSize_x())
            {

              /*
                prevent flagged blocks from being played:
                *y coordinate is inputted as a number (1-256)
                  must be decremented by one
              */
              if (!parseInput.getMoveCommand().commandAction &&
                  game->getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isCovered &&
                  game->getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isFlagged)
              {
                printInvalidInput("Block is flagged");
              }
              else
              {
                /* play block: */
                validInput = true;
                game->move(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1, parseInput.getMoveCommand().commandAction);

                /* check if game state changed: */
                switch (game->gameStatus())
                {
                case 0:
                  printBoard(*game); // << print revealed bombs
                  cout << ">>>> Game Over! You Lost <<<<" << endl;
                  printInstructions();
                  break;
                case 1:
                  printBoard(*game); // << print revealed bombs
                  cout << ">>>> Congratulations! You Won <<<<" << endl;
                  printInstructions();
                  break;
                }
              }
            }
            else
            {
              printInvalidInput(string("Out of bounds. Max(") + (char)(game->getSize_x() + 'a' - 1) + ", " + to_string(game->getSize_y()) + ")");
            }
          }
          else
          {
            // unrecognized command in this context
            printInvalidInput();
          }
        }
        break;
        }
      }
    }
  }

  cout << "Exiting" << endl;
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
  cout << "To start a new game, press 's'. To start at custom size, type 's x y'" << endl;
  cout << "To save and exit anytime, press 'x'" << endl;
  cout << "Game Play:" << endl;
  cout << "  To enter a column, use letters a-h. To enter a row, use numbers 1-8" << endl;
  cout << "  To dig, use 'd' and to flag, use 'f'" << endl;
  cout << "  Type everything into a single command with row-column first then action like this: '1a d' or 'b3 f'" << endl;
  cout << "To see these instructions again, press '?' anytime" << endl;
  cout << endl;
}

/*
    determine state of game and each block
    print in easy to read format and react well to scaling(larger x and y axis)

    inputs: (mineSweeper &) game object to be printed
 */
void printBoard(mineSweeper &game)
{
  /*
    determine how many digits is the largest y coordinate
  */
  uint8_t maxBlankSpace = 1;
  {
    int size_y_temp = game.getSize_y();
    while (size_y_temp /= 10)
      maxBlankSpace++;
  }

  /*
    print alphabetical x-axis located on the top of the board:
  */
  {
    for (uint8_t spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      cout << " ";

    for (int cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
      cout << char('A' + cnt_x) << "  ";
    cout << endl;
  }

  /*
    print each row one at a time.
    print y axis numbers simultaneously on left side:
  */
  for (int cnt_y = 0; cnt_y < game.getSize_y(); cnt_y++)
  {
    /*
      calc digit length for each y axis number:
    */
    uint8_t digitLen = 1;
    int cnt_y_temp = cnt_y + 1;
    while (cnt_y_temp /= 10)
    {
      digitLen++;
    }

    /*
      add spaces before each y axis number:
    */
    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex < maxBlankSpace - digitLen; spaceIndex++)
      {
        cout << " ";
      }
    }

    cout << cnt_y + 1 << ' '; // print y-axis number

    /*
      print each block state for the current row:
    */
    for (int cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {

      /*
        check game state,
        a won/lost game will reveal all blocks while a playing game will leave them covered
      */
      if (game.gameStatus() == 0 || game.gameStatus() == 1)
      {
        if (game.getBlock(cnt_x, cnt_y).isBomb)
        {
          /*
            print 'O' for a won game
            print 'X' for lost game:
          */
          if (game.gameStatus() == 1)
            cout << "O ";
          else
            cout << "X  ";
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
            cout << "O  ";
          else
            cout << char(45) << "  "; // char(45) is a dash https://upload.wikimedia.org/wikipedia/commons/1/1b/ASCII-Table-wide.svg
        }
        else
        {
          /*
            uncovered blocks can be one of 3 types
             - bomb
             - number hint
             - empty
           */
          if (game.getBlock(cnt_x, cnt_y).isBomb) // bomb block. this is redundant as this would mean game over already
            cout << "X  ";
          else if (game.getBlock(cnt_x, cnt_y).blockNum > 0) // number hint block
            cout << to_string(game.getBlock(cnt_x, cnt_y).blockNum) << "  ";
          else // empty block
            cout << "   ";
        }
      }
    }
    cout << endl;
  }

  /*
    reprint x-axis alphabetical key on the bottom for easy reading:
  */
  {
    for (uint8_t spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      cout << " ";

    for (int cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
      cout << char('A' + cnt_x) << "  ";
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
  cout << "Press '?' for help or try again" << endl;
}
