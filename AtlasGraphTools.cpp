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
    this->neighborCount = 0;
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

    return SUCCESS;

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
 ** Constructor for PriorityQueue Type
 ** Arguments is the goal node to be reached in this queue
 ************************************************************
 ************************************************************/

PriorityQueue::PriorityQueue(Node *goalNode) {
    this->goalNode = goalNode;
    this->count = 0;
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
 ** Function implementation for GetNodeAtIndex
 ** Takes the node index
 ** No special return codes
 ************************************************************
 ************************************************************/

Node* PriorityQueue::getNodeAtIndex(int index) const {
   if(index < this->count)
   return this->nodes[index];
   else {
       return (Node*)OUT_OF_BOUNDS;
   }
}

/***********************************************************
 ************************************************************
 ** Function implementation for getHeuristicAtIndex
 ** Takes the index
 ** No special return codes
 ************************************************************
 ************************************************************/

float PriorityQueue::getHeuristicAtIndex(int index) const {
   if(index < this->count)
   return this->heuristics[index];
   else {
       return OUT_OF_BOUNDS;
   }
}

/***********************************************************
 ************************************************************
 ** Function implementation for Insert
 ** Takes node to be added into queue
 ** Returns -2 if the node is already in the queue in a better path
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

            return -2;
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

    this->nodes.resize(this->count+1);
    this->heuristics.resize(this->count+1);

    for (index = 0; index < this->count; index++)
    {
        if (newHeuristic > heuristics[index])
        {
            insertIndex = index;
            break;
        }
    }

    for (index = this->count - 1; index >= insertIndex; index--)
    {
        this->nodes[index + 1] = this->nodes[index];
        this->heuristics[index + 1] = this->heuristics[index];
    }

    this->nodes[insertIndex] = node;
    this->heuristics[insertIndex] = newHeuristic;
    this->count++;
    return SUCCESS;
}
