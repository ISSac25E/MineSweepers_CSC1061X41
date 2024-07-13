#include "../header_files/mineSweeper.h"
using namespace std;

void mineSweeper::startNewGame(uint8_t size_x, uint8_t size_y)
{
  // minimume board size
  if (size_x < 1)
  {
    size_x = 1;
  }
  if (size_y < 1)
  {
    size_y = 1;
  }

  _boardSize_x = size_x;
  _boardSize_y = size_y;

  /*
    resize board to new size and setup bombs and numbers
  */
  _gameBoard.clear();
  _gameBoard.resize((int)_boardSize_x * (int)_boardSize_y); // make an empty board with default values

  _placeBombs();
  _placeNumbers();

  _gameStatus = 2;
}

void mineSweeper::move(uint8_t coord_x, uint8_t coord_y, bool action)
{
  _move(_getIndex(coord_x, coord_y), action);
  _updateGameStatus();
}

void mineSweeper::move(int index, bool action)
{
  _move(index, action);
  _updateGameStatus();
}

mineSweeper::block mineSweeper::getBlock(uint8_t coord_x, uint8_t coord_y)
{
  // Find the index of the block corresponding to the given coordinates
  int blockIndex = _getIndex(coord_x, coord_y);

  block retVal; // return block value. has default values normally
  if (blockIndex != -1 && _gameStatus != -1)
  {
    retVal = _gameBoard[blockIndex];
  }

  return retVal;
}

uint8_t mineSweeper::getSize_x()
{
  return _boardSize_x;
}

uint8_t mineSweeper::getSize_y()
{
  return _boardSize_y;
}

int8_t mineSweeper::getStatus()
{
  return _gameStatus;
}

void mineSweeper::_placeBombs()
{
  unsigned int indexCount;
  /**
   * set seed using time
   * we dont want the same bomb placement each time the game is launched
   *
   */
  srand(time(0));

  // run through each block and determine if a bomb needs to be placed:
  for (indexCount = 0; indexCount < _gameBoard.size(); indexCount++)
  {
    // a low '_bombSpread' num results in a greater chance of a bomb being placed at a given block:
    if (!(rand() % _bombSpread))
    {
      _gameBoard[indexCount].isBomb = true; // << set bomb
    }
  }
}

void mineSweeper::_placeNumbers()
{
  unsigned int blockIndex;

  for (blockIndex = 0; blockIndex < _gameBoard.size(); blockIndex++)
  {
    if (_gameBoard[blockIndex].isBomb) // Check if the block is a bomb
    {
      // if block is a bomb, increment all valid adjacent blocks
      /**
       * blockCode indicates with adjacent block is targeted
       * starting with top block and going around clockwise. eg.
       * blockcode = 0
       *  O X O
       *  O 0 O
       *  O O O
       * blockcode = 1
       *  O O X
       *  O 0 O
       *  O O O
       * ...thru 7
       *
       */
      uint8_t adjacentBlockCode;

      // iterate through each possible adjacent block code
      for (adjacentBlockCode = 0; adjacentBlockCode < 8; adjacentBlockCode++)
      {
        int adjacentIndex = _getAdjacent(blockIndex, adjacentBlockCode); // get index of adjacent block using blockCode

        if (adjacentIndex > -1) // valid adjacent block
        {
          _gameBoard[adjacentIndex].blockNum++; // << inc hint number
        }
      }
    }
  }
}

void mineSweeper::_updateGameStatus()
{
  bool allBlocksUncovered = true; // flag to track if all blocks are uncovered
  bool allBombsFlagged = true;    // flag to track if all bombs are flagged
  bool bombIsHit = false;         // flag to track if a bomb is hit

  unsigned int index;

  // iterate through each block in the game board
  for (index = 0; index < _gameBoard.size(); index++)
  {
    // check if a bomb block is uncovered, indicating a hit
    if (_gameBoard[index].isBomb && !_gameBoard[index].isCovered)
    {
      bombIsHit = true;
    }
    // check if a non-bomb block is covered, indicating not all blocks are uncovered
    if (!_gameBoard[index].isBomb && _gameBoard[index].isCovered)
    {
      allBlocksUncovered = false;
    }
    // check if the flags of bomb and non-bomb blocks are inconsistent, indicating not all bombs are flagged
    if ((_gameBoard[index].isBomb && !_gameBoard[index].isFlagged) ||
        (!_gameBoard[index].isBomb && _gameBoard[index].isFlagged && _gameBoard[index].isCovered))
    {
      allBombsFlagged = false;
    }
  }

  // update the game status accordingly
  if (bombIsHit)
  {
    // game won
    _gameStatus = 0;
  }
  else if (allBombsFlagged || allBlocksUncovered)
  {
    // game won
    _gameStatus = 1;
  }
}

void mineSweeper::_move(int index, bool action)
{
  // validate input, do nothing otherwise
  if (index >= 0 && index < _gameBoard.size())
  {
    if (action) // flag command
    {
      _gameBoard[index].isFlagged = !_gameBoard[index].isFlagged;
    }
    else // dig command
    {
      _gameBoard[index].isCovered = false; // uncover origin block by default

      // do adjacent block uncover only if origin block is not a bomb or hint block
      if (!_gameBoard[index].isBomb && _gameBoard[index].blockNum == 0)
      {
        // run through each of the 8 adjacent block and uncover if possible
        int8_t blockCode;
        for (blockCode = 0; blockCode < 8; blockCode++)
        {
          int adjacentIndex = _getAdjacent(index, blockCode);

          if (adjacentIndex != -1 && _gameBoard[adjacentIndex].isCovered) // validate adjacent block index
          {
            this->move(adjacentIndex, false); // call recursively on new block
          }
        }
      }
    }
  }
}

int mineSweeper::_getAdjacent(unsigned int index, uint8_t blockCode)
{
  // signed int because we might need to go into negatives
  // convert index into x and y coordinates:
  int16_t coord_y = index / _boardSize_x;
  int16_t coord_x = index % _boardSize_x;
  int returnIndex = 0;

  // make sure initial index is valid:
  if (index >= _gameBoard.size())
  {
    returnIndex = -1;
  }
  else
  {
    // use x and y to find adjacent based on blockcode:
    switch (blockCode)
    {
    case 0:
      coord_y--;
      // O X O <<
      // O 0 O
      // O O O
      break;
    case 1:
      coord_y--;
      coord_x++;
      // O O X <<
      // O 0 O
      // O O O
      break;
    case 2:
      coord_x++;
      // O O O
      // O 0 X <<
      // O O O
      break;
    case 3:
      coord_y++;
      coord_x++;
      // O O O
      // O 0 O
      // O O X <<
      break;
    case 4:
      coord_y++;
      // O O O
      // O 0 O
      // O X O <<
      break;
    case 5:
      coord_y++;
      coord_x--;
      // O O O
      // O 0 O
      // X O O <<
      break;
    case 6:
      coord_x--;
      // O O O
      // X 0 O <<
      // O O O
      break;
    case 7:
      coord_y--;
      coord_x--;
      // X O O <<
      // O 0 O
      // O O O
      break;
    default:
      returnIndex = -1; // invalid blockcode
      break;
    }
  }

  // determine return index value...
  if (returnIndex != -1)
  {
    if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize_x || coord_y >= _boardSize_y)
    {
      // out of bounds!
      returnIndex = -1;
    }
    else
    {
      // valid index:
      returnIndex = _getIndex(coord_x, coord_y);
    }
  }
  return returnIndex;
}

int mineSweeper::_getIndex(uint8_t coord_x, uint8_t coord_y)
{
  int returnIndex;
  if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize_x || coord_y >= _boardSize_y)
  {
    // out of bounds! invalid
    returnIndex = -1;
  }
  else
  {
    // valid, convert to index:
    returnIndex = ((int)coord_y * (int)_boardSize_x) + (int)coord_x;
  }

  return returnIndex;
}

mineSweeper::~mineSweeper()
{
  _gameBoard.clear();
}