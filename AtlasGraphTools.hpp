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

#define SUCCESS 1     // Indicates function executed succesfully
#define NULL_ARG -127 // Indicates a null pointer was passed
                      // as a function arg

/************************************************************
 ************************************************************
 ** Class Prototypes for AtlasGraphTools
 ************************************************************
 ************************************************************/

class Graph;
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

class Node
{
  friend class Graph;

private:
  point_t location;                     // Specifies the physical location of this node
  Graph *parent;                        // Specifies the parent graph this node is a part of.
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
  Node(float x, float y);                //Default Constructor for node with no graph
  Node(Graph *parent, float x, float y); //Constructor for node with a graph
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

std::ostream &operator<<(std::ostream &os, const Node &node);
std::ostream &operator<<(std::ostream &os, const Node *node);
float getNodeDistance(Node *node1, Node *node2);


/************************************************************
 ************************************************************
 ** Graph Class Definition
 ** This class is used to store nodes in Graphs
 ** Key data include the nodes physical location
 ** in cartesian space
 ** Has access to private members of Node class
 ************************************************************
 ************************************************************/

class Graph
{
private:
  int nodeCount;                                   // Stores the number of nodes in this graph
  std::vector<Node *> nodes;                       // Stores the nodes in this graph
  std::vector<std::vector<float> > nodeConnections; // The matrix reprentation
                                                   // Of this graph

public:
  //
  // Constructors
  //
  Graph(); // Default Constructor
  //
  // Member Functions
  //
  int addNode(Node *node); // Adds a node to the graph and updates matrix
  int updateConnections(); // Updates matrix reprentation for new connections
  int getNodeCount() const
  {
    return this->nodeCount;
  }
  std::vector<std::vector<float> > getNodeConnections() const
  {
    return this->nodeConnections;
  }
};

/************************************************************
 ************************************************************
 ** Nonmember functions for Graph Class
 ************************************************************
 ************************************************************/

std::ostream &operator<<(std::ostream &os, const Graph &graph);
std::ostream &operator<<(std::ostream &os, const Graph *graph);

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
};

#endif /* end of include guard: AtlasGraphTools_h */
