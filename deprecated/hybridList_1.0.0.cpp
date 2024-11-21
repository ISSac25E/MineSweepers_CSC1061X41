#include "../header_files/hybridList_1.0.0.h"

template <typename T>
hybridList<T>::hybridList(uint8_t subListLength) : _listLength(0)
{
  if (!subListLength)
    subListLength = 1;

  _subListLength = subListLength;
}

/*
  WORK IN PROGRESS!
*/
template <typename T>
bool hybridList<T>::insertAt(uint16_t index, T *data, uint16_t length)
{
  if (length)
  {
    if (index > _listLength)
      index = _listLength;

    /* move data */
    if (index < _listLength)
    {
      uint8_t offsetAmount = length % _subListLength;
      ListIndex_struct edgeIndex = _getListIndex(_listLength);

      /* copy data: */
      const uint16_t itemsToMove = _listLength - index;
      T moveSection[itemsToMove];
      {
        uint16_t moveIndex = index;
        uint16_t itemsMoved = 0;
        while (itemsMoved < itemsToMove)
        {
          ListIndex_struct compound_moveIndex = _getListIndex(moveIndex);
          uint16_t itemsToCopy = std::min(itemsToMove - itemsMoved, _subListLength - compound_moveIndex.subIndex);

          memcpy(&(moveSection[itemsMoved]),
                 &((_subLists_ll.getNodeData<T>(compound_moveIndex.node))[compound_moveIndex.subIndex]),
                 sizeof(T) * itemsToCopy);

          itemsMoved += itemsToCopy;
          moveIndex += itemsToCopy;
        }
      }

      /* shift data into new positions: */
      {
        uint16_t moveIndex = index;
        uint16_t itemsMoved = 0;
        while (itemsMoved < itemsToMove)
        {
          ListIndex_struct compound_moveIndex = _getListIndex(moveIndex);
          uint16_t itemsToCopy = std::min(itemsToMove - itemsMoved, _subListLength - compound_moveIndex.subIndex);

          memcpy(&((_subLists_ll.getNodeData<T>(compound_moveIndex.node))[compound_moveIndex.subIndex]),
                 &(moveSection[itemsMoved]),
                 sizeof(T) * itemsToCopy);

          itemsMoved += itemsToCopy;
          moveIndex += itemsToCopy;

          ListIndex_struct compound_moveIndex = _getListIndex(index + itemsMoved);
        }
      }

      /* Insert new data: */
      {
        uint16_t insertIndex = index;
        uint16_t itemsInserted = 0;
        while (itemsInserted < length)
        {
          ListIndex_struct compound_insertIndex = _getListIndex(insertIndex);
          uint16_t itemsToInsert = std::min(length - itemsInserted, _subListLength - compound_insertIndex.subIndex);

          if (compound_insertIndex.node >= _subLists_ll.nodeCount())
          {
            _subLists_ll.addNode<T>(_subLists_ll.nodeCount(), &(data[itemsInserted]), sizeof(T) * itemsToInsert);
          }
          else
          {
            memcpy(&((_subLists_ll.getNodeData<T>(compound_insertIndex.node))[compound_insertIndex.subIndex]),
                   &(data[itemsInserted]),
                   sizeof(T) * itemsToInsert);
          }

          itemsInserted += itemsToInsert;
          insertIndex += itemsToInsert;
        }
      }

      _listLength += length;
      return true;
    }
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
  returnVal.node = index / _subListLength;
  returnVal.subIndex = index % _subListLength;

  return returnVal;
}

template <typename T>
void hybridList<T>::_getNextSection(T *dataReturn, uint8_t indexOffset, uint8_t subListOffset)
{
  if ()
}