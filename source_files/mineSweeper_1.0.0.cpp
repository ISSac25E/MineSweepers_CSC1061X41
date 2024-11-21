#include "../header_files/mineSweeper_1.0.0.h"

mineSweeper::mineSweeper(uint8_t size_x, uint8_t size_y)
{
  _bombList.size = 0;

  /* validate input, must be a board size of at least 1 */
  if (!size_x)
    size_x = 1;
  if (!size_y)
    size_y = 1;

  _boardSize.x = size_x;
  _boardSize.y = size_y;

  /* create one node with all blocks marked as covered: */
  _firstBlockState = COVERED_BLOCK;
  /*
    no need to add the final node in the board game.
    The final node will be known because of the known board dimensions:
    _uncoveredBlocks.addNode<uint16_t>(0, _boardSize.x * _boardSize.y);
  */

  /*
    place bombs throughout board
    use srand to make sure different bombs layout each time
  */
  srand(time(0));
  _bombList.list = _setBombs(0, _bombList.size);
}

uint16_t *mineSweeper::_setBombs(uint16_t nextIndex, uint16_t &bombCnt)
{
  /*
    recursive function that will attempt to place a bomb
    if no bomb is placed, a new list is created and returned to calling functions for storage
  */

  uint16_t bombIndex = 0;
  bool bombFound = false;
  while (!bombFound && nextIndex < (_boardSize.x * _boardSize.y))
  {
    /*
      a small '_bombSpread' value will result in greater chances for bomb placement
    */
    if (!(rand() % _bombSpread))
    {
      bombIndex = nextIndex;
      bombFound = true;
      bombCnt++;
    }
    nextIndex++;
  }

  /*
    if bomb is not found, then the end of board has been reached
  */
  if (!bombFound)
  {
    /*
      the final function call...
      Allocate memory location and begin filling
    */
    if (bombCnt)
      return new uint16_t[bombCnt];
    else
      return nullptr;
  }
  else
  {
    /*
      if bomb has been placed, then call again
      retrieve allocated list and place bomb at designated index
    */
    uint16_t listIndex = bombCnt - 1; // this will be needed to store the bomb in the right place
    uint16_t *temp = _setBombs(nextIndex, bombCnt);

    if (temp)
      temp[listIndex] = bombIndex;

    return temp;
  }
}

void mineSweeper::move(uint16_t blockIndex, bool action)
{
  /*
    runs a move (flag or dig) on a block
    if digging, it will attempt to uncover as many blocks possible recursively
  */
  if (blockIndex < _boardSize.x * _boardSize.y)
  {
    if (action == BLOCK_ACTION_FLAG)
    {
      /*
        flag option,
          toggle flag at target index
        either adds or deletes node
      */
      int32_t flagIndex = _LLSearch(blockIndex, &_flagList);

      if (flagIndex > -1)
        _flagList.deleteNode(flagIndex);
      else
        _flagList.addNode<uint16_t>(0, blockIndex);
    }
    else
    {
      /*
        dig action
          uncover target block
          discover adjacent blocks and uncover as needed
        */
      _setBlockCoveredState(blockIndex, UNCOVERED_BLOCK);

      block currentBlock = getBlock(blockIndex);

      if (currentBlock.validBlock && !currentBlock.isBomb && !currentBlock.blockNum)
      {
        // run through each of the 8 adjacent block and uncover if possible
        // returns -1 if block does not exist based on x/y coordinates:
        for (uint8_t x = 0; x < 8; x++)
        {
          int32_t adjacentIndex = _getAdjacent(blockIndex, x);

          if (adjacentIndex != -1)
          {
            if (getBlock(adjacentIndex).isCovered && getBlock(adjacentIndex).validBlock)
              move(adjacentIndex, BLOCK_ACTION_DIG);
          }
        }
      }
    }
  }
}

uint8_t mineSweeper::gameStatus()
{
  bool allBlocksUncovered = true; // flag to track if all blocks are uncovered
  bool allBombsFlagged = true;    // flag to track if all bombs are flagged
  bool bombIsHit = false;         // flag to track if a bomb is hit

  for (uint16_t index = 0; index < _boardSize.x * _boardSize.y; index++)
  {
    block testBlock = getBlock(index);

    // check if a bomb block is uncovered, indicating a hit
    if (testBlock.isBomb && !testBlock.isCovered)
      bombIsHit = true;
    // check if a non-bomb block is covered, indicating not all blocks are uncovered
    if (!testBlock.isBomb && testBlock.isCovered)
      allBlocksUncovered = false;
    // check if the flags of bomb and non-bomb blocks are inconsistent, indicating not all bombs are flagged
    if ((testBlock.isBomb && !testBlock.isFlagged) ||
        (!testBlock.isBomb && testBlock.isFlagged && testBlock.isCovered))
      allBombsFlagged = false;
  }

  // update the game status accordingly
  if (bombIsHit)
    // game won
    return 0;
  else if (allBombsFlagged || allBlocksUncovered)
    // game won
    return 1;

  return 2; // still playing
}

mineSweeper::block mineSweeper::getBlock(uint16_t blockIndex)
{
  /*
    retrieve all block info and return
  */
  block returnBlock;
  returnBlock.validBlock = false;

  if (blockIndex < (_boardSize.x * _boardSize.y))
  {
    returnBlock.validBlock = true;
    returnBlock.blockNum = 0;

    returnBlock.isCovered = _getCoveredState(blockIndex);
    returnBlock.isBomb = _binaryArraySearch<uint16_t>(blockIndex, _bombList.list, _bombList.size) > -1;
    returnBlock.isFlagged = _LLSearch<uint16_t>(blockIndex, &_flagList) > -1;

    if (!returnBlock.isBomb)
    {
      // find any adjacent bombs ans update number:
      for (uint8_t x = 0; x < 8; x++)
      {
        int32_t adjacentIndex = _getAdjacent(blockIndex, x);

        if (adjacentIndex > -1) // validate adjacent index
        {
          if (_binaryArraySearch<uint16_t>(adjacentIndex, _bombList.list, _bombList.size) > -1)
            returnBlock.blockNum++;
        }
      }
    }
  }
  return returnBlock;
}

template <typename dataType>
int32_t mineSweeper::_binaryArraySearch(dataType search, dataType *data, uint16_t length)
{
  /*
    recursive binary search of SORTED array
  */
  if (!length)
    return -1; // Base case: empty subarray

  int32_t middleIndex = length / 2;

  if (data[middleIndex] == search)
    return middleIndex;
  else if (data[middleIndex] < search)
  {
    /*
      split second half of data array and search again
    */
    int32_t result = _binaryArraySearch(search, data + middleIndex + 1, length - middleIndex - 1);
    return (result == -1) ? -1 : middleIndex + 1 + result;
  }
  else
    /*
      split first half of data array and search again
    */
    return _binaryArraySearch(search, data, middleIndex);
}

template <typename dataType>
int32_t mineSweeper::_LLSearch(dataType search, linkedList *linkedList)
{
  /*
    linear linked-list search
      returns -1 if value does not exist
      returns index of first occurrence;
  */
  uint16_t nodeCount = linkedList->nodeCount();
  for (uint16_t x = 0; x < nodeCount; x++)
    if (search == *linkedList->getNodeData<dataType>(x))
      return x;
  return -1;
}

inline bool mineSweeper::_getCoveredState(uint16_t index)
{
  /*
    retrieve the covered state of a block.
  */
  bool returnValue = false;                  // << default return value
  if (index < (_boardSize.x * _boardSize.y)) // validate input
  {
    returnValue = _firstBlockState;

    uint16_t nodeCount = _uncoveredBlocks.nodeCount();
    uint16_t indexCount = 0;

    /*
      run through available nodes until target index reached
    */
    for (uint16_t x = 0; x < nodeCount; x++)
    {
      indexCount += *_uncoveredBlocks.getNodeData<uint16_t>(x);

      if (indexCount > index)
        break;

      returnValue = !returnValue;
    }
    /*
      this logic happens to work even if the last node does not exist
      return stored state from loop:
    */
  }
  return returnValue;
}

void mineSweeper::_setBlockCoveredState(uint16_t index, bool newState)
{
  if (index < (_boardSize.x * _boardSize.y))
  {
    bool blockState = _firstBlockState;
    uint16_t nodeCount = _uncoveredBlocks.nodeCount(); // keep track of total nodes
    uint16_t blockCount = 0;                           // total number of blocks traversed... waiting to reach or surpass 'index'
    uint16_t targetNode;                               // target node where 'index' is located

    ////// locate node, create new node if necessary //////
    {
      for (targetNode = 0; targetNode < nodeCount; targetNode++)
      {
        blockCount += *_uncoveredBlocks.getNodeData<uint16_t>(targetNode);

        if (blockCount > index)
          break;

        blockState = !blockState;
      }
      /* now we have 'targetNode' to split(if it exists) and the current state of the block */

      if (blockState == newState)
      {
        cout << "index: " << index << " -> setBlock: " << (blockState == COVERED_BLOCK ? "covered" : "uncovered") << " -> getBlock: " << (_getCoveredState(index) == COVERED_BLOCK ? "covered" : "uncovered") << endl;
        return; // state is already the same, no need to change
      }
      /*
        if 'targetNode' is >= 'nodeCount', then target index is outside linked-list's current range
         new node must be allocated and 'nodeCount' updated
      */
      if (targetNode >= nodeCount)
      {
        /* (_boardSize.x * _boardSize.y) - blockCount --> remaining blocks outside linked-list range */
        if (!_uncoveredBlocks.addNode<uint16_t>(nodeCount, (_boardSize.x * _boardSize.y) - blockCount))
        {
          return;
        }
        blockCount += ((_boardSize.x * _boardSize.y) - blockCount);
        nodeCount++;
      }
    }

    //////  split node //////
    {
      /*
        split overview example...
          *splitting for index 3

          linked-list:
            2 -> 4 -> 1 .....
                ^^^

          split list:
            2 -> 1 -> 1 -> 2 -> 1
                ^^^  ^^^  ^^^
                     ***
            4 split into 1 -> 1 -> 2

        it is always ideal if the index happens to reside at the edge of a node,
          as no new node have to be created(with the exception of front and back nodes)

        example with splitting for index 2:
          2 -> 4 -> 1 ...... 3 -> 3 -> 1
      */

      uint16_t *nodeDataPtr = _uncoveredBlocks.getNodeData<uint16_t>(targetNode);

      /*
        (blockCount - index) is the size of the right side of the split with the targeted block included
        *nodeDataPtr - (blockCount - index) is the size of the left side
      */
      if (!(*nodeDataPtr - (blockCount - index)))
      {
        // nothing left on the left side... merge left:
        if (!targetNode) // <<< starting at the very beginning of list
        {
          *nodeDataPtr -= 1;
          _uncoveredBlocks.addNode<uint16_t>(0, 1);
          _firstBlockState = !_firstBlockState;
        }
        else
        {
          *nodeDataPtr -= 1;
          (*_uncoveredBlocks.getNodeData<uint16_t>(targetNode - 1))++;
        }
      }
      else if (!(blockCount - index - 1))
      {
        // nothing left on the right side... merge right:
        if (targetNode >= (nodeCount - 1)) // <<< starting at the last node
        {
          *nodeDataPtr -= 1;
          _uncoveredBlocks.addNode<uint16_t>(nodeCount, 1);
        }
        else
        {
          *nodeDataPtr -= 1;
          (*_uncoveredBlocks.getNodeData<uint16_t>(targetNode + 1))++;
        }
      }
      else
      {
        /*
          worst case scenario
            node must be split into two new nodes
        */
        if (!_uncoveredBlocks.addNode<uint16_t>(targetNode, 1))
        {
          return;
        }
        if (!_uncoveredBlocks.addNode<uint16_t>(targetNode, *nodeDataPtr - (blockCount - index)))
        {
          return;
        }
        *nodeDataPtr = (blockCount - index) - 1;
      }
    }

    ////// remove empty nodes //////
    {
      /*
        remove empty nodes from head:
      */
      while (_uncoveredBlocks.nodeCount() && !(*_uncoveredBlocks.getNodeData<uint16_t>(0)))
      {
        _uncoveredBlocks.deleteNode(0);
        _firstBlockState = !_firstBlockState;
      }

      /*
        run through each node, delete and merge extra nodes:
      */

      /*
        'blockCount' will de used to determine wether final node is redundant.
        add head node initially:
      */
      uint16_t blockCount = *_uncoveredBlocks.getNodeData<uint16_t>(0);

      for (uint16_t x = 1; x < _uncoveredBlocks.nodeCount() - 1; x++)
      {
        if (!(*_uncoveredBlocks.getNodeData<uint16_t>(x))) // << empty node located, merge procedure:
        {
          (*_uncoveredBlocks.getNodeData<uint16_t>(x - 1)) += (*_uncoveredBlocks.getNodeData<uint16_t>(x + 1));
          // linked-list, cascade delete:
          _uncoveredBlocks.deleteNode(x);
          _uncoveredBlocks.deleteNode(x);
          x--;
        }
        blockCount += *_uncoveredBlocks.getNodeData<uint16_t>(x);
      }
      /* add tail node: */
      blockCount += *_uncoveredBlocks.getNodeData<uint16_t>(_uncoveredBlocks.nodeCount() - 1);

      /* delete final node if empty: */
      if (!(*_uncoveredBlocks.getNodeData<uint16_t>(_uncoveredBlocks.nodeCount() - 1)))
        _uncoveredBlocks.deleteNode(_uncoveredBlocks.nodeCount() - 1);
    }
  }
  return;
}

int32_t mineSweeper::_getAdjacent(uint16_t index, uint8_t blockCode)
{
  // signed int because we might need to go into negatives
  // convert index into x and y coordinates:
  int16_t coord_y = index / _boardSize.x;
  int16_t coord_x = index % _boardSize.x;
  int returnIndex = 0;

  // make sure initial index is valid:
  if (index >= _boardSize.x * _boardSize.y)
    return -1;
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
      return -1; // invalid blockcode
      break;
    }
  }

  // determine return index value...
  if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize.x || coord_y >= _boardSize.y)
  {
    // out of bounds!
    return -1;
  }
  else
  {
    // valid index:
    return _getIndex(coord_x, coord_y);
  }
}

int32_t mineSweeper::_getIndex(uint8_t coord_x, uint8_t coord_y)
{
  int returnIndex;
  if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize.x || coord_y >= _boardSize.y)
  {
    // out of bounds! invalid
    return -1;
  }
  else
  {
    // valid, convert to index:
    return ((int)coord_y * (int)_boardSize.x) + (int)coord_x;
  }
}
