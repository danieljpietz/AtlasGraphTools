#include "AtlasGraphTools.h"

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
}

/***********************************************************
 ************************************************************
 ** Constructor for Node Type with parent graph.
 ** Arguments are x and y position of node in cartesian space
 ************************************************************
 ************************************************************/

Node::Node(Graph *parent, double x, double y) {
    if (parent == (Node*) NULL) {
        return NULL_ARG;
    }
    this->location.x = x;
    this->location.y = y;
    parent->addNode(this);
}

/***********************************************************
 ************************************************************
 ** Function to add a neighbor to a node
 ** A neighbor is defined as a node that
 ** can be reached from this node
 ** Argument is the node to be added and distance to neightbor
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Node::addNeighbor(Node* neighbor, double distance) {
    if (neighbor == (Node*) NULL) {
        return NULL_ARG;
    }
    //
    // Add node and distance to respective vectors
    //
    this->neighbors.insert(this->neighbors.end(), neighbor);
    this->neighborDistances.insert(this->neighborDistances.end(), distance);
    //
    // Check if node is a member of a graph. If so, update the graph to
    // reflect the new connection
    //
    if (this->parent != NULL) {
        return this->parent->updateConnections();
    }

    return SUCCESS;
}

/***********************************************************
 ************************************************************
 ** Function to add a node to a graph
 ** Argument is the node to be added
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Graph::addNode(Node* node) {
    if (node == (Node*) NULL) {
        return NULL_ARG;
    }
    //
    // Assign a nodeID to the node
    //
    node->nodeID = this->nodes.size();
    //
    // Insert the node at the end of the nodes vector
    // Update graph structure matrix by adding a row and
    // column
    //
    this->nodes.insert(this->nodes.end(), node);

    std::vector<double> newRow;
    this->nodeConnections.insert(this->nodeConnections.end(), newRow);
    //
    // Iterate through all rows in matrix and add a column with
    // no connection
    //
    int i;
    for (i = 0; i < this->nodeConnections.size(); i++) {
        this->nodeConnections[i].resize(this->nodes.size(), -1);
    }
    //
    // Update connetions in grapg and return
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

    return SUCCESS;
}
