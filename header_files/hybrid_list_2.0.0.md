
This is an idea to improve the current hybrid list in an attempt to improve speed complexity within a large list

This class will aim to create a hybrid between linked lists and array lists
This will essentially be a series of lists linked together to create one seemingly continuous list

The User defines the size of each list. Smaller list results in faster insertions and larger lists result in better memory utilization.

'hybridList' requires the 'linkedList' class

here is an overview of a hybrid list:
[list 0] -> [list 1] -> [list 2]

list 0:
[slotsUsed, data slot 0, data slot 1, data slot 2... data slot (listSize)] -> [next list]

inserting data order of precedence
- it will check the current sub list for an available data slot
- it will check the previous sub list for an available data slot
- it will check the next sub list for an available data slot
- if no available memory slots are found, a new sub list will be inserted, essentially expanding the list size in that section 

this will allow for faster* insertion, especially when dealing with enormous lists

When deleting, a similar order of precedence will happen in an attempt to preserve memory
It will attempt to combine two nearby lists together.

The list will be faster at insertion and deletion compared to 'hybridList_1.x.x' at the expense of holes scattered throughout the lists resulting in wasted space.