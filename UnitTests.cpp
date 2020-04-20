#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Atlas/AtlasGraphTools.hpp"


using namespace std;


int main(int argc, char const* argv[]) {
    srand (time(NULL));
    int i; float rc;

    //
    // The Following Unit Tests checks neighbor functionality for 1M nodes
    // these nodes are generated at random and randomly assigned nodes
    // node pairs are then checked at random to connetivity
    //
    std::cout << "Beginning node connection verification test " << std::endl;

   Node* nodeN1 = new Node(0,0);
   Node* nodeN2 = new Node(0,0);

   rc = nodeN1->isNeighbor(nodeN2); // Show that the nodes are not neighbors
   assert(rc == 0);
   rc = nodeN2->isNeighbor(nodeN1); // Show that the nodes are not neighbors
   assert(rc == 0);
   rc = nodeN1->addNeighbor(nodeN2); // Add neighbor
   assert(rc == SUCCESS);
   rc = nodeN2->isNeighbor(nodeN1); // Show that the nodes are neighbors
   assert(rc == 1);
   rc = nodeN1->isNeighbor(nodeN2); // Show that the nodes are neighbors
   assert(rc == 1);
   rc = nodeN1->addNeighbor(nodeN2); // Show protection against double adding
   assert(rc == -2);
   rc = nodeN2->addNeighbor(nodeN1); // Show protection against double adding
   assert(rc == -2);
   std::cout << "Test Passed" << std::endl;

   std::cout << "Beginning node connection nullarg test " << std::endl;

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
    std::cout << "Beginning node distance function nullarg test" << std::endl;

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



    return 0;
}
