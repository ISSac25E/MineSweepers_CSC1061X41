#include "../header_files/linkedList_1.1.0.h"

linkedList::base_node::base_node() : nextNode(nullptr) {}

linkedList::node::node() : length(0) {}

linkedList::~linkedList()
{
  base_node *temp = &_headNode;

  while (temp->nextNode)
    deleteNode(0);
}

template <typename T>
bool linkedList::addNode(uint16_t index, T *data, int16_t length)
{
  if (length <= 0)
    return false; // invalid data length!

  uint8_t *newByteNode = new (nothrow) uint8_t[sizeof(node) + length];
  if (!newByteNode)
    return false;

  node *newNode = reinterpret_cast<node *>(newByteNode);
  newNode->length = length;

  memcpy(newByteNode + sizeof(node), data, length); // << write to the bytes trailing the node

  base_node *storeNode = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && storeNode->nextNode; nodeCnt++)
    storeNode = storeNode->nextNode;

  newNode->nextNode = storeNode->nextNode;
  storeNode->nextNode = newNode;
  return true;
}

void linkedList::deleteNode(uint16_t index)
{
  base_node *temp = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && temp->nextNode; nodeCnt++)
    temp = temp->nextNode;

  if (temp->nextNode)
  {
    uint8_t *deleteNode = reinterpret_cast<uint8_t *>(temp->nextNode);
    temp->nextNode = temp->nextNode->nextNode;

    delete[] deleteNode;
  }
}

template <typename T>
T *linkedList::getNodeData(uint16_t index)
{
  base_node *temp = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && temp->nextNode; nodeCnt++)
    temp = temp->nextNode;

  if (temp->nextNode)
  {
    uint8_t *byteData = reinterpret_cast<uint8_t *>(temp->nextNode);
    byteData += sizeof(node);

    return reinterpret_cast<T *>(byteData);
  }
  return nullptr;
}

int16_t linkedList::getNodeSize(uint16_t index)
{
  base_node *temp = &_headNode;

  for (uint16_t nodeCnt = 0; nodeCnt < index && temp->nextNode; nodeCnt++)
    temp = temp->nextNode;

  if (temp->nextNode)
  {
    return temp->nextNode->length;
  }
  return -1;
}

uint16_t linkedList::nodeCount()
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