#ifndef gameBoard_h
#define gameBoard_h

#define UNCOVERED_BLOCK 0
#define COVERED_BLOCK 1

#include <iostream>
#include <stdint.h>
#include <random>

#include "linkedList_1.1.0.h"

using namespace std;

class gameBoard
{
public:
  struct block
  {
    bool validBlock = false;

    uint8_t blockNum = 0; // ranges 0-8. 0 = no number
    bool isBomb = false;
    bool isFlagged = false;
  };

  /*
    constructor makes an empty game board with size 0x0 blocks
  */
  gameBoard();

  /*
    makes a new board with all covered blocks
    set size of board
    size 0 also allowed although useless
  */
  void newBoard(uint8_t size_x, uint8_t size_y);

  /*
    make a move on target index
    dig or flag action

    input: (uint16_t)target index, (bool) block action (true = dig, false = flag)
  */
  void move(uint16_t blockIndex, bool action);

  /*
    NO SAVE FUNCTIONALITY IN FINAL PROJECT
    batch uncover many blocks
    this is useful when preforming a large move to save time,
    OR
    when starting a new game with already uncovered blocks
  */
  // void uncoverBlocks(uint16_t *blocksToUncover, uint16_t length);

  /*
    returns a copy of the block based on inputted index
    if inputted index is out of range, 'validBlock' will be flagged
  */
  block getBlock(uint16_t blockIndex);

  /*
    Returns the state of the game

    -1 = idle, n/a
    0 = game lost
    1 = game won
    2 = game playing
  */
  int8_t gameStatus();

private:
  const uint8_t _bombSpread = 10; // larger number means more spread, must be larger than 0 -> (divide by 0 ERROR)

  bool _firstBlockState;
  linkedList _uncoveredBlocks;
  linkedList _flagList;
  struct
  {
    uint16_t *list = nullptr;
    uint16_t size = 0;
  } _bombList;

  struct
  {
    uint8_t x = 0;
    uint8_t y = 0;
  } _boardSize;

  /*
    set bombs using recursive function
    once end of board is reached, bomb list will be stored and function will return
    The reason why this function is recursive is because the number of bombs is not yet known.
    making a dynamic array and resizing would be inefficient.
    A recursive function does not need to know the amount of memory needed before recursing.

    input: (uint16_t) next index to potentially place a bomb, (uint16_t) number of bombs already created (returns number of bombs created)
    returns: (uint16_t*) array with correct amount of memory allocated.
  */
  uint16_t *_setBombs(uint16_t nextIndex, uint16_t &bombCnt);
};

#endif // #define gameBoard_h