#include "Node.h"
#include <iostream>


Node::Node(int row, int col, int dist_traveled)
{
    this-> row = row;
    this-> col = col;
    this-> dist_traveled = dist_traveled;
}

Node::~Node(){
    // TODO
}

int Node::getRow(){

    return this-> row;
}

int Node::getCol(){

    return this-> col;
}

int Node::getDistanceTraveled(){
    
    return this-> dist_traveled;
}

void Node::setDistanceTraveled(int dist_traveled)
{
    this-> dist_traveled = dist_traveled;
}

int Node::getEstimatedDist2Goal(Node* goal){
    
    int manhattanDistance = (this->getCol() - goal->getCol()) + (this->getRow() - goal->getRow());
    int distance = manhattanDistance + getDistanceTraveled();

    if (manhattanDistance < 0) {
        distance = (manhattanDistance * -1);
    }

    return distance;
}
    
//--------------------------------                             