/*
  changes from v1.0.0
    - not using templates for node sizes.
      it may have had compile-time type safety but it create a serious limitation when it came to
      using the linked list dynamically with the need for dynamic node sizes.
      As for storing data, templates will be used just the same for ease of type flexibility
*/

#ifndef linkedList_h
#define linkedList_h

#include <iostream>
#include <stdint.h>
#include <cstring>
using namespace std;

class linkedList
{
public:
  /*
    safely deallocates entire list.
  */
  ~linkedList();

  /*
    will add a single node at designated input

    If inputted number exceeds the number of nodes in the chain by more than 1,
    the new node will simply be added at the very end

    inputs: (uint16_t) location where to add new node, (T*)or(T) data to be added, (int16_t) data length (MUST BE POSITIVE NON-ZERO)
    returns: (bool) true if node was successfully allocated and proper input size, false otherwise
  */
  template <typename T>
  bool addNode(uint16_t index, T data)
  {
    return addNode(index, &data, sizeof(T));
  }
  template <typename T>
  bool addNode(uint16_t index, T *data, int16_t length);

  /*
    will delete a single node at designated input

    If inputted number exceeds the number of nodes in the chain,
    method will not delete any nodes and will return

    inputs: (uint16_t) location where to delete node
    returns: (void)

  */
  void deleteNode(uint16_t index);

  /*
    method returns type T of the node at designated index
    This pointer is essentially the contents of your node

    method will return null if node does not exist

    inputs: (uint16_t) location of target node
    returns: (T*) uint8_t array pointer(the contents of the node)
  */
  template <typename T>
  T *getNodeData(uint16_t index);

  /*
    get node size at specified index
    will return -1 if invalid index

    inputs: (uint16_t) node index
    outputs: (int16_t) node byte size, -1 if invalid index
  */
  int16_t getNodeSize(uint16_t index);

  /*
    traverses the list and returns number of nodes
  */
  uint16_t nodeCount();

private:
  struct node;
  struct base_node
  {
    node *nextNode;
    base_node();
  };
  struct node : base_node
  {
    int16_t length;
    node();
  };

  base_node _headNode;
};

#endif // #ifndef linkedList_h