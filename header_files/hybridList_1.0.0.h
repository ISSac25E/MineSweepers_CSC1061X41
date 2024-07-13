/*
  This class will aim to create a hybrid between linked lists and array lists
  This will essentially be a series of lists linked together to create one seemingly continuous list

  The User defines the size of each sub-list. Smaller sub-list results in less initial memory usage but worse memory efficiency at a large scale.

  'hybridList' requires the 'linkedList' class

  here is an overview of a hybrid list:
  [list 0] -> [list 1] -> [list 2]

  insertion and deletion will result in the standard O(n) time complexity as a result of shifting data around
  however, resizing the list is much faster as a result of having the data striped over multiple lists
  searching specific indexes is significantly faster especially with larger sub lists compared to a linked list solution
  */

#ifndef hybridList_h
#define hybridList_h

#include <iostream>
#include <stdint.h>
#include <cstring>

#include "linkedList_1.1.0.h"

using namespace std;

template <typename T>
class hybridList
{
public:
  /*
    there are no direct dynamic allocations in this class
    linked list has its own deconstructor so a deconstructor for this class is not needed
  */
  /*
    setup each sublist size
    Smaller sub-list results in less initial memory usage but worse memory efficiency at a large scale.

    inputs: (uint8_t) set sub-list size
  */
  hybridList(uint8_t subListSize);

  /*
    insert a list of data at a specified location.
    inserting data in bulk is much faster than one at a time

    inputs: (uint16_t) index to store at, (T *) data list, (uint16_t) list length (elements, not bytes)
    bool: true if successful, false otherwise
  */
  bool insertAt(uint16_t index, T *data, uint16_t length);

  /*
    delete multiple elements from list starting at given index
    deletion in bulk is faster than single
    if elements happens to delete outside list, excess will be ignored

    inputs: (uint16_t) index to delete starting at, (uint16_t) elements to delete
  */
  void deleteAt(uint16_t index, uint16_t length);

  /*
    returns actual pointer to element
    element can read and modified

    inputs: (uint16_t) index
    ouput: (T*) element pointer
  */
  T *getData(uint16_t index);

  /*
    returns list length in elements

    returns: (uint16_t) list length
  */
  uint16_t getListLength();

private:
  struct ListIndex_struct
  {
    uint16_t node;
    uint8_t subIndex;
  };

  /*
    input absolute index
    output struct with exact node and sub_index

    inputs: (uint16_t) index
    outputs: (ListIndex_struct) node and subIndex
  */
  ListIndex_struct _getListIndex(uint16_t index);
  linkedList _subLists;

  uint16_t _listLength;

  uint8_t _subListSize;
};

#endif // #ifndef hybridList_h