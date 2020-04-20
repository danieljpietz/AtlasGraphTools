#include "AtlasGraphTools.hpp"

/***********************************************************
 ************************************************************
 ** Constructor for Node Type without parent graph.
 ** Arguments are x and y position of node in cartesian space
 ************************************************************
 ************************************************************/

Node::Node(float x, float y)
{
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

Node::Node(Graph *parent, float x, float y)
{
    this->location.x = x;
    this->location.y = y;
    this->neighborCount = 0;
    if (parent != (Graph *)NULL)
    {
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
 **       -2: Indicated nodes are already neighbors
 ************************************************************
 ************************************************************/

int Node::addNeighbor(Node *neighbor)
{
    if (neighbor == (Node *)NULL)
    {
        return NULL_ARG;
    }

//
// This dependency has been removed, Processing will be purely linked list for speed
//

#if 0
    if (this->parent != neighbor->parent || this->parent == (Graph *)NULL)
    {
        return -1;
    }
#endif
    //
    // Check if the nodes are already neighbors
    //

    int i;
    for (i = 0; i < this->neighbors.size(); i++) {
        if (this->neighbors[i] == neighbor) {
            return -2;
        }
    }

    //
    //Calculate the distance between Nodes
    // No need to check return code on distance as
    // this and neighbor are known to be not null
    // At this point
    //
    float distance = getNodeDistance(this, neighbor);
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
    return SUCCESS;
#if 0
    return this->parent->updateConnections();
#endif
}

/***********************************************************
 ************************************************************
 ** Function to get the distance between two nodes
 ** Argument is two node pointers
 ** inlined to reduce function call overhead
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

float getNodeDistance(Node *node1, Node *node2)
{
    if (node1 == (Node *)NULL || node2 == (Node *)NULL)
    {
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

int Node::isNeighbor(Node *node)
{
    if (node == (Node *)NULL)
    {
        return NULL_ARG;
    }
    int i;
    for (i = 0; i < (int)this->neighbors.size(); i++)
    {
        if (node == this->neighbors[i])
        {
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

std::ostream &operator<<(std::ostream &os, const Node &node)
{
    os << "Node @ (" << node.getLocation().x << ',' << node.getLocation().y << ')'
       << " with " << node.getNeighborCount() << " neighbors";
    return os;
}

/***********************************************************
 ************************************************************
 ** Overloaded print stream operator for Node pointer
 ************************************************************
 ************************************************************/

std::ostream &operator<<(std::ostream &os, const Node *node)
{
    return os << *node;
}

/***********************************************************
 ************************************************************
 ** Constructor for Graph
 ** No Arguments
 ************************************************************
 ************************************************************/

Graph::Graph()
{
    this->nodeCount = 0;
}

/***********************************************************
 ************************************************************
 ** Function to add a node to a graph
 ** Argument is pointer to the node to be added
 ** Special Return Codes: None
 ************************************************************
 ************************************************************/

int Graph::addNode(Node *node)
{
    if (node == (Node *)NULL)
    {
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

    std::vector<float> newRow(this->nodes.size(), -1);


    this->nodeConnections.insert(this->nodeConnections.end(), newRow);
    //
    // Iterate through all rows in matrix and add a column with
    // no connection
    //
    int i;
    for (i = 0; i < (int)this->nodeConnections.size(); i++)
    {
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

int Graph::updateConnections()
{
    int i, j;
    for (i = 0; i < this->getNodeCount(); i++)
    {
        this->nodeConnections[i][i] = 0;
        for (j = 0; j < i; j++)
        {
            if (this->nodes[i]->isNeighbor(this->nodes[j]))
            {
                this->nodeConnections[i][j] = this->nodeConnections[j][i] = getNodeDistance(this->nodes[i], this->nodes[j]);
            }
            else
            {
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

std::ostream &operator<<(std::ostream &os, const Graph &graph)
{
    int i, j;
    os << "Graph with " << graph.getNodeCount() << " nodes" << std::endl;
    os << std::fixed << "|" << graph.getNodeConnections()[0][0];
    for (j = 1; j < graph.getNodeCount(); j++)
    {
        os << " " << graph.getNodeConnections()[0][j];
    }
    os << "|";
    for (i = 1; i < graph.getNodeCount(); i++)
    {
        os << std::endl
           << "|" << graph.getNodeConnections()[i][0];
        for (int j = 1; j < graph.getNodeCount(); j++)
        {
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

std::ostream &operator<<(std::ostream &os, const Graph *graph)
{
    return os << *graph;
}

/***********************************************************
 ************************************************************
 ** Constructor for PriortyQueue
 ** Arugment is goal node for the path
 ************************************************************
 ************************************************************/

PriorityQueue::PriorityQueue(Node *goalNode)
{
    this->count = 0;
    this->goalNode = goalNode;
}

/***********************************************************
 ************************************************************
 ** Function implementation for GetNodeIndex
 ** Takes node to be indexed
 ** Returns -1 if the node is not in the queue
 ************************************************************
 ************************************************************/

int PriorityQueue::getNodeIndex(Node *node)
{
    if (node == (Node *)NULL) //Check for null input argument
    {
        return NULL_ARG;
    }

    int i;

    for (i = 0; i < this->count; i++)
    {
        if (node == this->nodes[i])
        {
            return i;
        }
    }

    return -1;
}

/***********************************************************
 ************************************************************
 ** Function implementation for Insert
 ** Takes node to be added into queue
 ** No Special Return Codes
 ************************************************************
 ************************************************************/

int PriorityQueue::insert(Node *node, float pathLength)
{
    if (node == (Node *)NULL) //Check for null input argument
    {
        return NULL_ARG;
    }

    //
    //  Calculate the remaining distance to the goal node
    //

    float remainingDistance = getNodeDistance(node, this->goalNode);
    float newHeuristic = pathLength + remainingDistance;

    //
    // Check and see if the node exists in the queue
    //

    int nodeIndex = this->getNodeIndex(node);

    //
    // This path is taken if the if the node is in the queue
    //

    if (nodeIndex != -1)
    {
        if (newHeuristic > this->heuristics[nodeIndex])
        {
            //
            // Node has already been visited with a shorter path. No action necesary.
            //

            return 1;
        }
        else
        {

            //
            // Node has already been visited with a longer path. Remove the node and before adding again
            //

            //this->removeNode(nodeIndex);
        }
    }

    int index;
    int insertIndex = this->count;

    for (index = 0; index < this->count; index++)
    {
        if (newHeuristic < heuristics[index])
        {
            insertIndex = index;
            break;
        }
    }

    for (; index < this->count; index++)
    {
        this->nodes[index + 1] = this->nodes[index];
        this->heuristics[index + 1] = this->heuristics[index];
    }

    this->nodes[insertIndex] = node;
    this->heuristics[insertIndex] = newHeuristic;
    return 1;
}
