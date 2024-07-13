#include "../header_files/hybridList_1.0.0.h"

template <typename T>
hybridList<T>::hybridList(uint8_t subListSize) : _listLength(0)
{
  if (!subListSize)
    subListSize = 1;

  _subListSize = subListSize;
}

template <typename T>
bool hybridList<T>::insertAt(uint16_t index, T *data, uint16_t length)
{
  if (length)
  {
    ListIndex_struct edgeIndex = _getListIndex(_listLength); 
    const uint8_t currentListAvailableSize = _listLength % _subListSize;
  }
  return true;
}

template <typename T>
void hybridList<T>::deleteAt(uint16_t index, uint16_t length)
{
}

template <typename T>
void hybridList<T>::deleteAt(uint16_t index, uint16_t length)
{
}

template <typename T>
T *hybridList<T>::getData(uint16_t index)
{
}

template <typename T>
uint16_t hybridList<T>::getListLength()
{
  return _listLength;
}

template <typename T>
hybridList<T>::ListIndex_struct hybridList<T>::_getListIndex(uint16_t index)
{
  ListIndex_struct returnVal;
  returnVal.node = index / _subListSize;
  returnVal.subIndex = index % _subListSize;

  return returnVal;
}
