#ifndef gameBoard_h
#define gameBoard_h

#include <iostream>
#include <stdint.h>

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
  void newBoard(uint16_t boardSize);

  /*
    set bomb indexes.
    indexes are not verified,
    they are simply stored.

    returns true if allocated and stored, false otherwise
  */
  bool setBombs(uint16_t *bombIndexes, uint16_t length);

  /*
    uncovers a single block
    can be time consuming if uncovering many blocks using this method
  */
  void uncoverBlock(uint16_t blockIndex);

  /*
    batch uncover many blocks
    this is useful when preforming a large move to save time,
    OR
    when starting a new game with already uncovered blocks
  */
  void uncoverBlocks(uint16_t *blocksToUncover, uint16_t length);

  /*
    returns a copy of the block based on inputted index
    if inputted index is out of range, 'validBlock' will be flagged
  */
  block getBlock(uint16_t blockIndex);
};

#endif // #define gameBoard_h