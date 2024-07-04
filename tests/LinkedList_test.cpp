#include <iostream>
#include "../source_files/linkedList.cpp"
using namespace std;

int main()
{
  // Create a linked list with node size of 16 bytes
  linkedList<16> list;

  // Test 1: Add nodes to the list, must be less than or equal to 16 bytes
  int intData1 = 12345;
  int intData2 = 67890;
  double doubleData = 3.14;
  char charData[16] = "Hello";

  bool added1 = list.addNode(0, intData1);   // Add int data at index 0
  bool added2 = list.addNode(1, intData2);   // Add int data at index 1
  bool added3 = list.addNode(2, doubleData); // Add double data at index 2
  bool added4 = list.addNode(3, charData);   // Add char array data at index 3

  cout << "Test 1: Add nodes to the list" << endl;
  cout << "Added node 1: " << (added1 ? "Success" : "Fail") << endl;
  cout << "Added node 2: " << (added2 ? "Success" : "Fail") << endl;
  cout << "Added node 3: " << (added3 ? "Success" : "Fail") << endl;
  cout << "Added node 4: " << (added4 ? "Success" : "Fail") << endl;

  // Test 2: Get nodes from the list
  int *getIntData1 = list.getNode<int>(0);
  int *getIntData2 = list.getNode<int>(1);
  double *getDoubleData = list.getNode<double>(2);
  char *getCharData = list.getNode<char>(3);

  cout << "\nTest 2: Get nodes from the list" << endl;
  cout << "Node 0: " << (getIntData1 ? to_string(*getIntData1) : "null") << endl;
  cout << "Node 1: " << (getIntData2 ? to_string(*getIntData2) : "null") << endl;
  cout << "Node 2: " << (getDoubleData ? to_string(*getDoubleData) : "null") << endl;
  cout << "Node 3: " << (getCharData ? getCharData : "null") << endl;

  // Test 3: Delete nodes from the list
  list.deleteNode(1); // Delete node at index 1

  cout << "\nTest 3: Delete nodes from the list" << endl;
  getDoubleData = list.getNode<double>(1);
  cout << "Node 1 after deletion: " << (getDoubleData ? to_string(*getDoubleData) : "null") << endl;

  // Test 4: Node count
  cout << "\nTest 4: Node count" << endl;
  cout << "Number of nodes in the list: " << list.nodeCount() << endl;

  // Test 5: Add node out of bounds
  cout << "\nTest 5: Add node out of bounds" << endl;
  cout << "Added out of bounds node: " << (list.addNode(100, 0xFF) ? "Success" : "Fail") << endl;

  cout << "\nTest 4: Node count" << endl;
  cout << "Number of nodes in the list: " << list.nodeCount() << endl;

  // Test 6: Add node at the end of the list
  int endData = 55555;
  bool addedAtEnd = list.addNode(list.nodeCount(), endData);

  cout << "\nTest 6: Add node at the end of the list" << endl;
  cout << "Added node at the end: " << (addedAtEnd ? "Success" : "Fail") << endl;
  int *getEndData = list.getNode<int>(list.nodeCount() - 1);
  cout << "Node at end: " << (getEndData ? to_string(*getEndData) : "null") << endl;

  // Insertion Test 1: Add node in the middle of the list
  int midData = 11111;
  bool addedMid = list.addNode(2, midData); // Add node at index 2

  cout << "\nInsertion Test 1: Add node in the middle of the list" << endl;
  cout << "Added node at index 2: " << (addedMid ? "Success" : "Fail") << endl;
  int *getMidData = list.getNode<int>(2);
  cout << "Node at index 2: " << (getMidData ? to_string(*getMidData) : "null") << endl;

  // Insertion Test 2: Add node at the beginning of the list
  int beginData = 22222;
  bool addedBegin = list.addNode(0, beginData); // Add node at index 0

  cout << "\nInsertion Test 2: Add node at the beginning of the list" << endl;
  cout << "Added node at index 0: " << (addedBegin ? "Success" : "Fail") << endl;
  int *getBeginData = list.getNode<int>(0);
  cout << "Node at index 0: " << (getBeginData ? to_string(*getBeginData) : "null") << endl;

  // Large Number of Nodes Test: Add and delete a large number of nodes using a for loop
  cout << "\nLarge Number of Nodes Test: Add and delete a large number of nodes using a for loop" << endl;
  for (int i = 0; i < 1000; ++i)
  {
    list.addNode(i, i);
  }

  cout << "Added 1000 nodes" << endl;
  cout << "Current node count: " << list.nodeCount() << endl;

  for (int i = 999; i >= 0; --i)
  {
    list.deleteNode(i);
  }

  cout << "Deleted 1000 nodes" << endl;
  cout << "Current node count: " << list.nodeCount() << endl;

  return 0;
}