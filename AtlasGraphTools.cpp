#include "AtlasGraphTools.hpp"

/***********************************************************
 ************************************************************
 ** Constructor for Node Type without parent graph.
 ** Arguments are x and y position of node in cartesian space
 ************************************************************
 ************************************************************/

Node::Node(double x, double y) {
    this->nodeID = -1;
    this->location.x = x;
    this->location.y = y;
    this->parent = NULL;
    this->neighborCount = 0;
}

/***********************************************************
 ************************************************************
 ** Constructor for Node Type with parent graph.
 ** Arguments are x and y position of node in cartesian space
 ************************************************************
 ************************************************************/

Node::Node(Graph *parent, double x, double y) {
    this->location.x = x;
    this->location.y = y;
    this->neighborCount = 0;
    if (parent != (Graph*) NULL) {
        parent->addNode(this);
    }
    else {

    }
}

/***********************************************************
 ************************************************************
 ** Function to add a neighbor to a node
 ** A neighbor is defined as a node that
 ** can be reached from this node
 ** Only Parent Graph will be updated
 ** Argument is the node to be added and distance to neightbor
 ** Special Return Codes:
 **       -1: Indicates Nodes had different parent Graphs
 ************************************************************
 ************************************************************/

int Node::addNeighbor(Node* neighbor) {
    if (neighbor == (Node*) NULL) {
        return NULL_ARG;
    }
    if (this->parent != neighbor->parent || this->parent == (Graph*) NULL) {
        return -1;
    }
    //
    //Calculate the distance between Nodes
    // No need to check return code on distance as
    // this and neighbor are known to be not null
    // At this point
    //
    double distance = getNodeDistance(this, neighbor);
    //
    // Add node and distance to respective vectors
    //
    this->neighbors.insert(this->neighbors.end(), neighbor);
    this->neighborDistances.insert(this->neighborDistances.end(), distance);
    this->neighborCount++;

    neighbor->neighbors.insert(neighbor->neighbors.end(), this);
    neighbor->neighborDistances.insert(neighbor->neighborDistances.end(), distance);
    neighbor->neighborCount++;

    //
    // update the parent graph reflect the new connection
    //

    return this->parent->updateConnections();

}

/***********************************************************
 ************************************************************
 ** Function to get the distance between two nodes
 ** Argument is two node pointers
 ** inlined to reduce function call overhead
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

 inline double getNodeDistance(Node* node1, Node* node2) {
     if (node1 == (Node*) NULL || node2 == (Node*) NULL) {
       return NULL_ARG;
     }
     return sqrt((node1->getLocation().x - node2->getLocation().x) *
                 (node1->getLocation().x - node2->getLocation().x) +
                 (node1->getLocation().y - node2->getLocation().y) *
                 (node1->getLocation().y - node2->getLocation().y));
 }

/***********************************************************
 ************************************************************
 ** Function to check if a node neighbors another node
 ** A neighbor is defined as a node that
 ** can be reached from this node
 ** Argument is potential neighbor
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Node::isNeighbor(Node* node) {
    if (node == (Node*) NULL) {
        return NULL_ARG;
    }
    int i;
    for (i = 0; i < this->neighbors.size(); i++) {
        if (node == this->neighbors[i]) {
            return 1;
        }
    }
    return 0;
}

/***********************************************************
 ************************************************************
 ** Overloaded print stream operator for Node reference
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << "Node @ (" << node.getLocation().x << ',' << node.getLocation().y << ')'
       << " with " << node.getNeighborCount() << " neighbors";
    return os;
}

/***********************************************************
 ************************************************************
 ** Overloaded print stream operator for Node pointer
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Node* node) {
    return os << *node;
}

/***********************************************************
 ************************************************************
 ** Constructor for Graph
 ** No Arguments
 ************************************************************
 ************************************************************/

Graph::Graph() {
  this->nodeCount = 0;
}

/***********************************************************
 ************************************************************
 ** Function to add a node to a graph
 ** Argument is pointer to the node to be added
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Graph::addNode(Node* node) {
    if (node == (Node*) NULL) {
        return NULL_ARG;
    }
    //
    // Assign a nodeID and parent to the node
    //
    node->nodeID = this->nodes.size();
    node->parent = this;
    //
    // Insert the node at the end of the nodes vector
    // Update graph structure matrix by adding a row and
    // column
    //
    this->nodes.insert(this->nodes.end(), node);

    std::vector<double> newRow(this->nodes.size(), 0);

    this->nodeConnections.insert(this->nodeConnections.end(), newRow);
    //
    // Iterate through all rows in matrix and add a column with
    // no connection
    //
    int i;
    for (i = 0; i < this->nodeConnections.size(); i++) {
        this->nodeConnections[i].resize(this->nodes.size(), 0);
    }
    this->nodeCount++;
    //
    // Update connetions in graph and return
    // corresponding return code
    //
    return this->updateConnections();
}

/***********************************************************
 ************************************************************
 ** Function to update node connections in a graph
 ** No Arguments
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Graph::updateConnections() {
    int i, j;
    for (i = 0; i < this->getNodeCount(); i++) {
      this->nodeConnections[i][i] = 0;
      for (j = 0; j < i; j++) {
        if (this->nodes[i]->isNeighbor(this->nodes[j])) {
          this->nodeConnections[i][j] = this->nodeConnections[j][i] = getNodeDistance(this->nodes[i], this->nodes[j]);
        }
        else {
          this->nodeConnections[i][j] = this->nodeConnections[j][i] = 0;
        }
      }
    }
    return SUCCESS;
}

/***********************************************************
 ************************************************************
 ** Overloaded print stream operator for Graph reference
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    int i, j;
    os << "Graph with " << graph.getNodeCount() << " nodes" << std::endl;
    os << std::fixed << "|" << graph.getNodeConnections()[0][0];
    for (j = 1; j < graph.getNodeCount(); j++) {
        os << " " << graph.getNodeConnections()[0][j];
    }
    os << "|";
    for (i = 1; i < graph.getNodeCount(); i++) {
        os << std::endl << "|" << graph.getNodeConnections()[i][0];
        for (int j = 1; j < graph.getNodeCount(); j++) {
            os << " " << graph.getNodeConnections()[i][j];
        }
        os << "|";
    }
    return os;
}

/***********************************************************
 ************************************************************
 ** Overloaded print stream operator for Graph pointer
 ************************************************************
 ************************************************************/

std::ostream& operator<<(std::ostream& os, const Graph* graph) {
    return os << *graph;
}
