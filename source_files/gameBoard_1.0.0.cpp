#include "../header_files/gameBoard_1.0.0.h"

gameBoard::gameBoard()
{
  _boardSize.x = 0;
  _boardSize.y = 0;
}

void gameBoard::newBoard(uint8_t size_x, uint8_t size_y)
{
  /* clear all previous board data */
  while (_uncoveredBlocks.nodeCount())
    _uncoveredBlocks.deleteNode(0);

  while (_flagList.nodeCount())
    _flagList.deleteNode(0);

  if (_bombList.list)
    delete[] _bombList.list;
  _bombList.size = 0;

  /* validate input */
  if (!size_x)
    size_x = 1;
  if (!size_y)
    size_y = 1;

  _boardSize.x = size_x;
  _boardSize.y = size_y;

  _firstBlockState = COVERED_BLOCK;
  /* create one node with all blocks marked as covered: */
  _uncoveredBlocks.addNode<uint16_t>(0, _boardSize.x * _boardSize.y);

  srand(time(0));
  _bombList.list = _setBombs(0, _bombList.size);
}

/*
  TODO: test function thoroughly
*/
uint16_t *gameBoard::_setBombs(uint16_t nextIndex, uint16_t &bombCnt)
{
  uint16_t bombIndex = 0;
  bool bombFound = false;
  while (!bombFound && nextIndex < (_boardSize.x * _boardSize.y))
  {
    if (!(rand() % _bombSpread))
    {
      bombIndex = nextIndex;
      bombFound = true;
      bombCnt++;
    }
    nextIndex++;
  }

  if (!bombFound)
  {
    /*
      we are the chosen function call!
      Allocate memory location and begin filling!
    */
    if (bombCnt)
      return new uint16_t[bombCnt];
    else
      return nullptr;
  }
  else
  {
    uint16_t listIndex = bombCnt - 1; // this will be needed to store the bomb in the right place
    uint16_t *temp = _setBombs(nextIndex, bombCnt);

    if (temp)
      temp[listIndex] = bombIndex;

    return temp;
  }
}