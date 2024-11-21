/*
  Amir Gorkovchenko - CSC1061X41 Capstone

  this is the game engine class.
  each instance of this class is a new game.

  This uses a separate list for bombs, flags, and covered blocks
  bombs are implemented using an array
  flags are implemented using a linked list
  covered blocks are implemented using a linked list

  rather than using individual indexes for covered blocks,
   it uses a sort of run-length encoding where more identical blocks in sequence results in better memory efficiency
*/

#ifndef mineSweeper_h
#define mineSweeper_h

#define UNCOVERED_BLOCK 0
#define COVERED_BLOCK 1

#define BLOCK_ACTION_DIG 0
#define BLOCK_ACTION_FLAG 1

#include <iostream>
#include <stdint.h> // << for 'uint8_t'
#include <ctime> // << for random number seed generation
#include <random>

#include "linkedList_1.1.0.h"

using namespace std;

class mineSweeper;              /* DEBUG: for debugging purposes */
void printBoard(mineSweeper &); /* DEBUG: for debugging purposes */

class mineSweeper
{
  friend void printBoard(mineSweeper &); /* DEBUG: for debugging purposes */

public:
  /*
    this will be used when processing a block index and
    returning from method
  */
  struct block
  {
    bool validBlock = false;

    uint8_t blockNum = 0; // ranges 0-8. 0 = no number
    bool isBomb = false;
    bool isFlagged = false;
    bool isCovered = UNCOVERED_BLOCK;
  };

  /*
    makes a new board with all covered blocks
    set size of board
    size 0 also allowed although useless and results in game won
  */
  mineSweeper(uint8_t size_x, uint8_t size_y);

  /*
    make a move on target index
    dig or flag action

    input: (uint16_t)target index, (bool) block action (true = dig, false = flag)
  */
  void move(uint16_t blockIndex, bool action);
  void move(uint8_t x, uint8_t y, bool action)
  {
    move(_getIndex(x, y), action);
  }

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

  block getBlock(uint8_t x, uint8_t y)
  {
    return getBlock(_getIndex(x, y));
  }

  /*
    Returns the width of the game board
   */
  uint8_t getSize_x()
  {
    return _boardSize.x;
  }
  /*
     Returns the height of the game board.
   */
  uint8_t getSize_y()
  {
    return _boardSize.y;
  }

  /*
    Returns the state of the game

    0 = game lost
    1 = game won
    2 = game playing
  */
  uint8_t gameStatus();

private:
  const uint8_t _bombSpread = 20; // larger number means more spread, must be larger than 0 -> (divide by 0 ERROR)

  /*
    '_firstBlockState' and '_uncoveredBlocks' are used together to determine the covered state of the entire board
    first block state of the run-length encoding
      eg:
        _boardSize.x == 3
        _boardSize.y == 3
        _firstBlockState == UNCOVERED_BLOCK
        _uncoveredBlocks --> (node 0)==2 --> (n 1)==1 --> (n 2)==1

        will result in:
          U U C
          U C C
          C C C

        *final node is not required as the remaining blocks will simply assume the same state
  */
  bool _firstBlockState;
  linkedList _uncoveredBlocks;

  /*flag list stored as individual indexes*/
  linkedList _flagList;

  /*
    bomb list stored as individual indexes, list length determined at board creation
  */
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

  /*
    binary searches an ORDERED linked list for a specific value

    if value is located, the corresponding index will be returned. Otherwise, a negative will be returned

    input: data to locate (dataType), data array to search(dataType *), length of data array(uint16_t)
    output: index of located data, -1 otherwise
  */
  template <typename dataType>
  static int32_t _binaryArraySearch(dataType search, dataType *data, uint16_t length);

  /*
    searches the linkedlist for a value
    since this is a linkedlist, binary search is not an option.
    The last element will always take n operations to simply reach even

    I understand that without a proper iterator to manage the linked list traversal, this is anyways going to be slow
    I'll just add that to my ever growing pile of todo's :)
    TODO: implement proper linked-list iterator for efficient(er) searching

    input: data to locate (dataType), linked list to search(linkedList *)
    output: index of located data, -1 otherwise
  */
  template <typename dataType>
  static int32_t _LLSearch(dataType search, linkedList *linkedList);

  /*
    gets the block state at specified index
    an invalid input will result in false(bool)
  */
  inline bool _getCoveredState(uint16_t index);

  /*
    sets block covered state at designated index
    use 'UNCOVERED_BLOCK' 'COVERED_BLOCK' to set accordingly
  */
  void _setBlockCoveredState(uint16_t index, bool newState);

  /*
    Determines the index of the adjacent block based on the index and blockCode parameters.

    Inputs: (uint16_t) the index of the block for which adjacent block index is to be obtained, (uint8_t) a block code (0-7) representing one of the eight adjacent blocks
    Returns the index of the adjacent block if it exists and is within the game board boundaries;
            otherwise, returns -1 to indicate an invalid adjacent block.

   */
  int32_t _getAdjacent(uint16_t index, uint8_t blockCode);

  /*
    determines the index of the block in the one-dimensional game board based on its x-y coordinates.

    Input: (uint8_t) coord_x - the x-coordinate of the block, (uint8_t) coord_y - the y-coordinate of the block
    Returns the index of the block if it exists within the game board boundaries;
            otherwise, returns -1 to indicate an invalid block index.

   */
  int32_t _getIndex(uint8_t coord_x, uint8_t coord_y);
};

#endif // #define mineSweeper_h