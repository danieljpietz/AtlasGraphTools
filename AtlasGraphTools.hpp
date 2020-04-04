#ifndef AtlasGraphTools_h
#define AtlasGraphTools_h
#include <vector>
#include <iostream>
#include <syslog.h>
#include <math.h>
/************************************************************
 ************************************************************
 ** Type Declarations for AtlasGraphTools
 ************************************************************
 ************************************************************/

typedef struct {double x; double y;} point_t; // Represents a 2-dimensional
                                              // point in cartesian space

/************************************************************
 ************************************************************
 ** Error Code Returns for AtlasGraphTools
 ************************************************************
 ************************************************************/

#define SUCCESS 1   // Indicates function executed succesfully
#define NULL_ARG -127  // Indicates a null pointer was passed
                    // as a function arg

/************************************************************
 ************************************************************
 ** Class Prototypes for AtlasGraphTools
 ************************************************************
 ************************************************************/

class Graph;
class Node;

/************************************************************
 ************************************************************
 ** Node Class Definition
 ** This class is used to store nodes in Graphs
 ** Key data include the nodes physical location
 ** in cartesian space
 ** Graph Class has access private access
 ************************************************************
 ************************************************************/

class Node {
    friend class Graph;
private:
    point_t location; // Specifies the physical location of this node
    Graph* parent; // Specifies the parent graph this node is a part of.
    unsigned int nodeID; // Specifies the index of this node in its parent graph
    std::vector<Node *> neighbors; // Nodes this node has a conection to
    std::vector<double> neighborDistances; // Distance to a given node relative
                                           // to neighbors array. -1 indicates
                                           //no connection
    int neighborCount; // Number of neighbor connectios to this node
public:
    //
    // Constructors
    //
    Node (double x, double y); //Default Constructor for node with no graph
    Node (Graph* parent, double x, double y); //Constructor for node with a graph
    int isNeighbor(Node* node); // Checks if node neighbors this
    //
    // Member Functions
    //
    int addNeighbor(Node* neighbor); //Add a connection to this
                                                      //node
    point_t getLocation() const { // Returns the physical location of the node
      return this->location;      // Inlined to eliminate function call overhead
    }

    std::vector<Node *> getNeighbors() const { // Returns the neighbors of the node
      return this->neighbors;                  // Inlined to eliminate function call overhead
    }

    int getNeighborCount() const {  // Returns the number of neighbor connectios to this node
      return this->neighborCount;   // Inlined to eliminate function call overhead
    }
};

/************************************************************
 ************************************************************
 ** Nonmember functions for Node Class
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Node& node);
std::ostream& operator<<(std::ostream& os, const Node* node);
inline double getNodeDistance(Node* node1, Node* node2);
/************************************************************
 ************************************************************
 ** Graph Class Definition
 ** This class is used to store nodes in Graphs
 ** Key data include the nodes physical location
 ** in cartesian space
 ** Has access to private members of Node class
 ************************************************************
 ************************************************************/

class Graph {
private:
    int nodeCount; // Stores the number of nodes in this graph
    std::vector<Node*> nodes; // Stores the nodes in this graph
    std::vector<std::vector<double> > nodeConnections; // The matrix reprentation
                                                       // Of this graph

public:
    //
    // Constructors
    //
    Graph (); // Default Constructor
    //
    // Member Functions
    //
    int addNode(Node* node); // Adds a node to the graph and updates matrix
    int updateConnections(); // Updates matrix reprentation for new connections
    int getNodeCount () const {
      return this->nodeCount;
    }

    std::vector<std::vector<double> > getNodeConnections() const {
      return this->nodeConnections;
    }

};

/************************************************************
 ************************************************************
 ** Nonmember functions for Graph Class
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Graph& graph);
std::ostream& operator<<(std::ostream& os, const Graph* graph);

#endif /* end of include guard: AtlasGraphTools_h */
