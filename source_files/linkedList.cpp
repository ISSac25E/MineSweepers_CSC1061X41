#include "../header_files/linkedList.h"

template <size_t NodeSize>
linkedList<NodeSize>::base_node::base_node() : nextNode(nullptr) {}

template <size_t NodeSize>
linkedList<NodeSize>::node::node()
{
  memset(data, 0, NodeSize);
}

template <size_t NodeSize>
linkedList<NodeSize>::~linkedList()
{
  base_node *temp = &_headNode;

  while (temp->nextNode)
    deleteNode(0);
}

template <size_t NodeSize>
template <typename T>
bool linkedList<NodeSize>::addNode(uint16_t index, T *data)
{
  node *newNode = new (nothrow) node();
  if (!newNode)
    return false;

  memcpy(newNode->data, data, NodeSize);  // we dont care if we write a little extra data or even a little garbage

  base_node *storeNode = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && storeNode->nextNode; nodeCnt++)
    storeNode = storeNode->nextNode;

  newNode->nextNode = storeNode->nextNode;
  storeNode->nextNode = newNode;
  return true;
}

template <size_t NodeSize>
void linkedList<NodeSize>::deleteNode(uint16_t index)
{
  base_node *temp = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && temp->nextNode; nodeCnt++)
    temp = temp->nextNode;

  if (temp->nextNode)
  {
    node *deleteNode = temp->nextNode;
    temp->nextNode = temp->nextNode->nextNode;

    delete deleteNode;
  }
}

template <size_t NodeSize>
template <typename T>
T *linkedList<NodeSize>::getNode(uint16_t index)
{
  base_node *temp = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && temp->nextNode; nodeCnt++)
    temp = temp->nextNode;

  if (temp->nextNode)
  {
    return reinterpret_cast<T *>(temp->nextNode->data);
  }
  return nullptr;
}

template <size_t NodeSize>
uint16_t linkedList<NodeSize>::nodeCount()
{
  base_node *temp = &_headNode;
  uint16_t nodeCnt = 0;

  while (temp->nextNode)
  {
    temp = temp->nextNode;
    nodeCnt++;
  }

  return nodeCnt;
}