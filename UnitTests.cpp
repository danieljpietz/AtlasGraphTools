#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Atlas/AtlasGraphTools.hpp"


using namespace std;


int main(int argc, char const* argv[]) {
    srand (time(NULL));
    int i, j; float rc;

    /***********************************************
    ************************************************
    The following unit tests test all functions include
    in Node class
    ************************************************
    ***********************************************/

    //
    // The Following Unit Tests checks neighbor functionality for 1M nodes
    // these nodes are generated at random and randomly assigned nodes
    // node pairs are then checked at random to connetivity
    //
    std::cout << "Beginning node connection verification test: ";

    Node* nodeN1 = new Node(0,0);
    Node* nodeN2 = new Node(0,0);
    Node* nodeN3 = new Node(0,0);

    rc = nodeN1->isNeighbor(nodeN2); // Show that the nodes are not neighbors
    assert(rc == 0);
    rc = nodeN2->isNeighbor(nodeN1); // Show that the nodes are not neighbors
    assert(rc == 0);
    rc = nodeN1->addNeighbor(nodeN2); // Add neighbor
    assert(rc == SUCCESS);
    rc = nodeN1->isNeighbor(nodeN3); // Show that the nodes are not neighbors
    assert(rc == 0);
    rc = nodeN1->addNeighbor(nodeN3); // Add neighbor
    assert(rc == SUCCESS);
    rc = nodeN1->addNeighbor(0x0); // Add a null neighbor
    assert(rc == NULL_ARG);
    rc = nodeN2->isNeighbor(nodeN1); // Show that the nodes are neighbors
    assert(rc == 1);
    rc = nodeN1->isNeighbor(nodeN2); // Show that the nodes are neighbors
    assert(rc == 1);
    rc = nodeN1->addNeighbor(nodeN2); // Show protection against double adding
    assert(rc == -2);
    rc = nodeN2->addNeighbor(nodeN1); // Show protection against double adding
    assert(rc == -2);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning node connection nullarg test: ";

    Node *noden1 = new Node(0,0);
    Node *noden2 = new Node(0,0);

    rc = noden1->isNeighbor(noden2); // 2 Legal Nodes
    assert(rc != NULL_ARG);

    rc = noden1->isNeighbor(0x0); // An illegal node in the second arg
    assert(rc == NULL_ARG);

    std::cout << "Test Passed" << std::endl;


    //
    // The Following Unit Tests checks the GetNodeDistance Function
    // for 10,000,000 pairs of nodes
    //

    std::cout << "Beginning node distance function verification for 10M nodes" << std::endl;
    for (i = 0; i < 10000000; i++) {
        float x1 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        float y1 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        float x2 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        float y2 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        Node *nodeD1 = new Node(x1,y1);
        Node *nodeD2 = new Node(x2,y2);
        float nodeDistance = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        float rc = getNodeDistance(nodeD1, nodeD2);
        assert(rc != NULL_ARG);    //Check for null argument
        assert(rc == nodeDistance); // Check that the distances are the same
    }
    std::cout << "Test passed" << std::endl;

    //
    // The Following Unit Tests shows crashing for null arg on the distance function
    //
    std::cout << "Beginning node distance function nullarg test: ";

    Node *noded1 = new Node(0,0);
    Node *noded2 = new Node(0,0);

    rc = getNodeDistance(noded1, noded2); // 2 Legal Nodes
    assert(rc != NULL_ARG);

    rc = getNodeDistance(noded1, 0x0); // An illegal node in the second arg
    assert(rc == NULL_ARG);

    rc = getNodeDistance(0x0, noded1); // An illegal node in the first arg
    assert(rc == NULL_ARG);

    rc = getNodeDistance(0x0, 0x0); // 2 illegal nodes
    assert(rc == NULL_ARG);

    std::cout << "Test Passed" << std::endl;



    /***********************************************
    ************************************************
    The following unit tests test all functions include
    in PriorityQueue class
    ************************************************
    ***********************************************/

    std::cout << "Beginning PriorityQueue Tests" << std::endl;

    const int queueSize = 15;

    Node *queueNodes[queueSize];
    queueNodes[0] = new Node(0,0); // Insert a known shortest node
    Node *goalNode = new Node(25,25); // Insert a known goal node
    PriorityQueue *queue = new PriorityQueue(goalNode);
    rc = queue->insert(queueNodes[0], -getNodeDistance(queueNodes[0], goalNode)); // Force this node to be the shortest path
    assert(rc == SUCCESS);
    rc = queue->insert(0x0, 1); //Insert a bad address
    std::cout << "Beginning Insert nullarg test: ";
    assert(rc == NULL_ARG);
    std::cout << "Test Passed" << std::endl;

    //
    // Generate 15 random nodes and add them to the queue
    //
    std::cout << "Beginning Positive Insert Test" << std::endl;
    for (i = 1; i < queueSize; i++) {
        float x1 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        float y1 = -100 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200)));
        queueNodes[i] = new Node(x1,y1);
        rc = queue->insert(queueNodes[i], static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(200))));
        assert(rc == SUCCESS);
        assert(queue->getNodeCount() == i+1);
    }
    std::cout << "Test Passed" << std::endl;
    //
    // Assign a random node to be connected to the goal
    //

    queueNodes[rand() % queueSize]->addNeighbor(goalNode);

    //
    // Give each node up to 5 neighbors
    //

    for (i = 0; i < queueSize; i++) {
       int neighborCount = rand() % 5;
       if (queueNodes[i]->getNeighborCount() > 4) {
            continue;
        }
       for (j = 0; j < neighborCount; j++) {
           rc = queueNodes[i]->addNeighbor(queueNodes[rand() % queueSize]);
           assert(rc == SUCCESS || rc == -2); // Allow success and duplicate entries
       }
    }

    std::cout << "Beginning getIndex nullarg test: ";
    rc = queue->getNodeIndex(0x0);
    assert(rc == NULL_ARG);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning PriorityQueue sorted test: ";
    for (i = 0; i < queueSize-1; i++) {
        assert(queue->getHeuristicAtIndex(i) > queue->getHeuristicAtIndex(i+1));
    }
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning PriorityQueue indexing out of bounds test: ";
    assert(queue->getNodeAtIndex(queueSize + 1) == (Node*)OUT_OF_BOUNDS);
    assert(queue->getHeuristicAtIndex(queueSize + 1) == OUT_OF_BOUNDS);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning PriorityQueue positive indexing test: ";
    assert(queue->getNodeAtIndex(queueSize-1) == queueNodes[0]);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning queue reinsert test: ";
    rc = queue->insert(queueNodes[0], 300);
    assert(rc == -2);
    rc = queue->insert(queueNodes[0], -getNodeDistance(queueNodes[0], goalNode) - 1);
    assert(rc == SUCCESS);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning queue insert nullarg test: ";
    rc = queue->removeNode((Node*)0x0);
    assert(rc == NULL_ARG);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning queue remove OUT_OF_BOUNDS test: ";
    rc = queue->removeNode(queueSize + 1);
    assert(rc == OUT_OF_BOUNDS);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning queue false remove test: ";
    Node *nodeNotInQueue = new Node(0,0);
    rc = queue->removeNode(nodeNotInQueue);
    assert(rc == -1);
    std::cout << "Test Passed" << std::endl;


    std::cout << "Beginning queue remove test: ";
    Node* nodeToRemove = queue->getNodeAtIndex(0);
    int previousCount = queue->getNodeCount();
    rc = queue->removeNode(nodeToRemove);
    assert(rc == SUCCESS);
    assert(queue->getNodeCount() == previousCount - 1);
    rc = queue->insert(nodeToRemove, -getNodeDistance(nodeToRemove, goalNode) - 1);
    assert(rc == SUCCESS);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning queue pop test: ";
    previousCount = queue->getNodeCount();
    Node* lastNode = queue->getNodeAtIndex(previousCount-1);
    Node* poppedNode = queue->pop();
    assert(lastNode == poppedNode);
    assert(queue->getNodeCount() == previousCount-1);
    std::cout << "Test Passed" << std::endl;

    std::cout << "Beginning empty queue pop test: ";
    PriorityQueue *emptyQueue = new PriorityQueue(goalNode);
    poppedNode = emptyQueue->pop();
    assert(poppedNode == (Node*)OUT_OF_BOUNDS);

    return 0;
}
