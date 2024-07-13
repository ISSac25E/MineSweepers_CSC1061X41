#ifndef mineSweeper_h
#define mineSweeper_h

/*
  amir gorkovchenko
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctime>    // << for random number seed generation
#include <stdint.h> // << for 'uint8_t'
using namespace std;

class mineSweeper
{
public:
  /*
    struct for each individual block
      holds the state of each block
      contains default states of each block
      this struct is used by the game vector

   */
  struct block
  {
    bool isCovered = true;
    bool isFlagged = false;

    bool isBomb = false;
    uint8_t blockNum = 0;
  };

  /*
    start a new game, input x and y coordinates
    Initializes a new game by setting the size of the game board, clearing the existing game board,
    placing bombs randomly on the board, placing numbers indicating the number of adjacent bombs for each non-bomb block
  */
  void startNewGame(uint8_t, uint8_t);

  /*
    Opens a file to load a saved game,
    reads game board size, parses game board data from the file.
    updates the game state accordingly, start game

    returns: Returns true if the saved game was successfully loaded and parsed, otherwise returns false
   */
  // bool startSavedGame(); TODO

  /*
    Saves the current game state to a file if the game status indicates a completed game.
    if the game is completed, opens the file and writes game board data to it, otherwise deletes the file
   */
  // void saveGame(); TODO

  /*
    checks if a saved game file exists with game board size information.
    opens the file, reads game board size, and returns true if size data exists.

    returns: (bool) true if a saved game file exists with game board size information, otherwise false.

   */
  // bool checkSavedGame(); TODO

  /*
    converts coordinates into index and runs parent _move function
    updates the game-play status

    inputs: coord_x, coord_y: the coordinates of the block to perform the move on
    action: a bool indicating whether to flag the block (true) or uncover it (false)
  */
  void move(uint8_t, uint8_t, bool);
  /*
    runs parent _move function
    updates the game-play status

    inputs: (int) the index of the block to perform the move on, (bool) indicating whether to flag the block (true) or uncover it (false)
   */
  void move(int, bool);

  /*
    retrieves the block struct at the specified coordinates on the game board.
    finds the index of the block corresponding to the given coordinates.
    if the block index is valid and the game is not in an uninitialized state,
    retrieves the block data from the game board.

    input: (uint8_t) coord_x - the x-coordinate of the block, (uint8_t) coord_y - the y-coordinate of the block
    returns: the block data as a struct at the specified coordinates,
             OR a default block if the coordinates are invalid or the game is uninitialized.

   */
  block getBlock(uint8_t, uint8_t);

  /*
    Returns the width of the game board.
   */
  uint8_t getSize_x();
  /*
     Returns the height of the game board.
   */
  uint8_t getSize_y();

  /*
    Returns the state of the game

    -1 = idle, n/a
    0 = game lost
    1 = game won
    2 = game playing
   */
  int8_t getStatus();

  // clear vector
  ~mineSweeper();

private:
  // vector with all blocks:
  vector<block> _gameBoard;

  const char *_fileName = "cache.dat"; // file where game data is stored
  const uint8_t _bombSpread = 10;      // larger number means more spread, must be larger than 0!
  uint8_t _boardSize_x;
  uint8_t _boardSize_y;

  int8_t _gameStatus = -1; // -1 na, 1 = game won, 0 = game lost, 2 = playing

  /*
    Iterates over the elements of _gameBoard vector, generating a random number for each element
    and setting its isBomb flag to true if the random number modulo _bombSpread evaluates to true.
  */
  void _placeBombs();

  /*
    Iterates over each block in the _gameBoard vector. For each block that contains a bomb,
    it determines the adjacent blocks and increments their blockNum value, indicating the number
    of adjacent bombs.
  */
  void _placeNumbers();
  /*
    Checks the current state of the game board to determine the game status.
    Sets _gameStatus to 0 if a bomb is uncovered, to 1 if all bombs are correctly flagged
    or all non-bomb blocks are uncovered; otherwise, leaves _gameStatus unchanged.
   */
  void _updateGameStatus();
  /*
    Flags or uncovers the block specified by the index parameter.
    If the action is to uncover a non-bomb block with no adjacent bombs, recursively uncovers
    adjacent blocks until a block with adjacent bombs is encountered.

    Inputs: index (int) of the block to perform the action on, action (bool) indicating whether to flag the block (true) or uncover it (false)
   */
  void _move(int, bool);

  /*
    Process: Determines the index of the adjacent block based on the index and blockCode parameters.

    Inputs: (uint) the index of the block for which adjacent block index is to be obtained, (uint8_t) a block code (0-7) representing one of the eight adjacent blocks
    Returns the index of the adjacent block if it exists and is within the game board boundaries;
            otherwise, returns -1 to indicate an invalid adjacent block.

   */
  int _getAdjacent(unsigned int, uint8_t);
  /*
    Process: Determines the index of the block in the one-dimensional game board vector based on its coordinates.
    
    Input: (uint8_t) coord_x - the x-coordinate of the block, (uint8_t) coord_y - the y-coordinate of the block
    Returns the index of the block if it exists within the game board boundaries;
            otherwise, returns -1 to indicate an invalid block index.

   */
  int _getIndex(uint8_t, uint8_t);
};

#endif // #ifndef mineSweeper_h