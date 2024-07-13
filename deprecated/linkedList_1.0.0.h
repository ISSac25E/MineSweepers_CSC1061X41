#ifndef linkedList_h
#define linkedList_h

#include <iostream>
#include <stdint.h>
#include <cstring>
using namespace std;

template <size_t NodeSize>
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

    inputs, (uint16_t) location where to add new node, (T*) data to be added
    returns, (bool) true if node was successfully allocated, false otherwise
  */
  template <typename T>
  bool addNode(uint16_t index, T data)
  {
    return addNode(index, &data);
  }
  template <typename T>
  bool addNode(uint16_t index, T *data);

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
  T *getNode(uint16_t index);

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
    uint8_t data[NodeSize];
    node();
  };

  base_node _headNode;
};

#endif