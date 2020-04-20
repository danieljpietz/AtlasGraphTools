#ifndef AtlasGraphTools_h
#define AtlasGraphTools_h

/************************************************************
 ************************************************************
 ** OS-Independent includes
 ************************************************************
 ************************************************************/

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <math.h>

/************************************************************
 ************************************************************
 ** OS-Dependent includes
 ************************************************************
 ************************************************************/
#ifndef __WIN32
//#include <syslog.h>
#endif

/************************************************************
 ************************************************************
 ** Type Declarations for AtlasGraphTools
 ************************************************************
 ************************************************************/

typedef struct
{
  float x;
  float y;
} point_t; // Represents a 2-dimensional
           // point in cartesian space

/************************************************************
 ************************************************************
 ** Code Returns for AtlasGraphTools
 ************************************************************
 ************************************************************/

#define SUCCESS (1)     // Indicates function executed succesfully
#define NULL_ARG (-127) // Indicates a null pointer was passed
                      // as a function arg
#define OUT_OF_BOUNDS (-255) // Indicates out of bounds indexing

/************************************************************
 ************************************************************
 ** Class Prototypes for AtlasGraphTools
 ************************************************************
 ************************************************************/

class Node;
class PriorityQueue;

/************************************************************
 ************************************************************
 ** Node Class Definition
 ** This class is used to store nodes in Graphs
 ** Key data include the nodes physical location
 ** in cartesian space
 ** Graph Class has access private access
 ************************************************************
 ************************************************************/

class Node  {
private:
  point_t location;                     // Specifies the physical location of this node
  unsigned int nodeID;                  // Specifies the index of this node in its parent graph
  std::vector<Node *> neighbors;        // Nodes this node has a conection to
  std::vector<float> neighborDistances; // Distance to a given node relative
                                        // to neighbors array
  int neighborCount;                    // Number of neighbor connections to this node
  Node *next;
  Node *previous;

public:
  //
  // Constructors
  //
  Node(float x, float y);                //Default Constructor for node
  //
  // Member Functions
  //
  int isNeighbor(Node *node);      // Checks if node neighbors this
  int addNeighbor(Node *neighbor); //Add a connection to this node

  point_t getLocation() const
  {                        // Returns the physical location of the node
    return this->location; // Inlined to eliminate function call overhead
  }

  std::vector<Node *> getNeighbors() const
  {                         // Returns the neighbors of the node
    return this->neighbors; // Inlined to eliminate function call overhead
  }

  int getNeighborCount() const
  {                             // Returns the number of neighbor connectios to this node
    return this->neighborCount; // Inlined to eliminate function call overhead
  }

  Node *getNext() const
  {
    return this->next;
  }

  Node *getPrevious() const
  {
    return this->previous;
  }
};

/************************************************************
 ************************************************************
 ** Nonmember functions for Node Class
 ************************************************************
 ************************************************************/

float getNodeDistance(Node *node1, Node *node2);

/************************************************************
 ************************************************************
 ** Priority Class Definition
 ** This class is used to store nodes in a min-last array
 ** Key data include the nodes, the number of nodes, and their
 ** heuristic sizes
 ************************************************************
 ************************************************************/

class PriorityQueue
{
private:
  Node *goalNode;
  std::vector<Node *> nodes;
  std::vector<float> heuristics;
  int count;

public:
  PriorityQueue(Node *goalNode);
  int insert(Node *node, float pathLength);
  Node *pop();
  int removeNode(Node *node);
  int removeNode(int index);
  int getNodeIndex(Node *node);
  Node* getNodeAtIndex(int index) const;
  float getHeuristicAtIndex(int index) const;
};

#endif /* end of include guard: AtlasGraphTools_h */
