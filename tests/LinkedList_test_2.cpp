#include <iostream>
#include "../source_files/linkedList_1.1.0.cpp"

using namespace std;

// Helper function to print node data (for demonstration purposes)
template <typename T>
void printNodeData(T *data, int16_t length)
{
  cout << "Node data [" << length << "]: ";
  int count = length / sizeof(T);
  for (int i = 0; i < count; ++i)
  {
    cout << data[i] << " ";
  }
  cout << endl;
}

int main()
{
  // Create an instance of the linked list
  linkedList list;

  // Test node insertion
  cout << "Testing node insertion..." << endl;
  int data1[] = {1, 2, 3};
  if (list.addNode(list.nodeCount(), data1, sizeof(data1)))
  {
    cout << "Node inserted successfully at position 0." << endl;
  }
  else
  {
    cout << "Failed to insert node at position 0." << endl;
  }

  if (list.addNode(list.nodeCount(), 4))
  {
    cout << "Node inserted successfully at position 1." << endl;
  }
  else
  {
    cout << "Failed to insert node at position 1." << endl;
  }

  if (list.addNode(list.nodeCount(), 5))
  {
    cout << "Node inserted successfully at position 2." << endl;
  }
  else
  {
    cout << "Failed to insert node at position 2." << endl;
  }

  const char *testDat = "HelloWorld";
  if (list.addNode<const char>(list.nodeCount(), testDat, strlen(testDat) + 1))
  {
    cout << "Node inserted successfully at position 3." << endl;
  }
  else
  {
    cout << "Failed to insert node at position 3." << endl;
  }

  // Test node retrieval
  cout << "\nTesting node retrieval..." << endl;
  auto *node1Data = list.getNodeData<int>(0);
  if (node1Data != nullptr)
  {
    cout << "Node at position 0: ";
    printNodeData(node1Data, list.getNodeSize(0));
  }
  else
  {
    cout << "Failed to retrieve node at position 0." << endl;
  }

  auto *node2Data = list.getNodeData<int>(1);
  if (node2Data != nullptr)
  {
    cout << "Node at position 1: ";
    printNodeData(node2Data, list.getNodeSize(1));
  }
  else
  {
    cout << "Failed to retrieve node at position 1." << endl;
  }

  auto *node3Data = list.getNodeData<int>(2);
  if (node3Data != nullptr)
  {
    cout << "Node at position 2: ";
    printNodeData(node3Data, list.getNodeSize(2));
  }
  else
  {
    cout << "Failed to retrieve node at position 2." << endl;
  }

  char *node4Data = (list.getNodeData<char>(3));
  if (node4Data != nullptr)
  {
    cout << "Node at position 3[" << list.getNodeSize(3) << "] " << node4Data << endl;
  }
  else
  {
    cout << "Failed to retrieve node at position 3." << endl;
  }

  // Test node deletion
  cout << "\nTesting node deletion..." << endl;
  list.deleteNode(1); // Delete node at position 1
  if (list.getNodeData<int>(1) == nullptr)
  {
    cout << "Node at position 1 successfully deleted." << endl;
  }
  else
  {
    cout << "Failed to delete node at position 1." << endl;
  }

  // Test edge cases
  cout << "\nTesting edge cases..." << endl;
  // Add node beyond current size
  if (list.addNode(10, 6))
  {
    cout << "Node inserted successfully at position 10." << endl;
  }
  else
  {
    cout << "Failed to insert node at position 10." << endl;
  }

  // Test large allocation/deallocation
  cout << "\nTesting large allocation/deallocation..." << endl;
  const int numNodes = 1000; // Adjust as needed
  for (int i = 0; i < numNodes; ++i)
  {
    if (list.addNode(i, i))
    {
      // cout << "Node inserted successfully at position " << i << "." << endl;
    }
    else
    {
      cout << "Failed to insert node at position " << i << "." << endl;
    }
  }

  // Check node count
  cout << "Total nodes in the list: " << list.nodeCount() << endl;

  // Success message
  cout << "\nAll tests completed." << endl;

  return 0;
}
