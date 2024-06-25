#include "PathSolver.h"
#include <iostream>

PathSolver::PathSolver(){
    
    nodesExplored = new NodeList[NODE_LIST_ARRAY_MAX_SIZE];
    openList = new NodeList[NODE_LIST_ARRAY_MAX_SIZE];
}

PathSolver::~PathSolver(){

    delete[] nodesExplored;
    delete[] openList;
}

void PathSolver::forwardSearch(Env env){

    Node* currentNode = nullptr;
    Node* goalNode = nullptr;
    bool reachedGoal = false;

    for (int i = 0; i < ENV_DIM; i++) {
        for (int j = 0; j < ENV_DIM; j++) {

            if (env[i][j] == SYMBOL_START) {
                currentNode = new Node(i,j,0);

                openList->addElement(currentNode);
            }
            
            if (env[i][j] == SYMBOL_GOAL) {
                goalNode = new Node(i,j,0);
            }
        }
    } 

    int count = 1;
    do {
        //std::cout << "iteration " << count << ":" << currentNode->getRow() << "," << currentNode->getCol() << "," << currentNode->getDistanceTraveled() << std::endl;

        int row = currentNode->getRow();
        int col = currentNode->getCol();
        int distance = currentNode->getDistanceTraveled();
        int shortestDist = ENV_DIM*ENV_DIM;
  
        for (int i = 0; i < openList->getLength(); i++) {

            Node* node = openList->getNode(i);

            if (!existsInList(node->getRow(), node->getCol(), nodesExplored)) {

                int estimatedDist = node->getEstimatedDist2Goal(goalNode);

                if (estimatedDist < shortestDist) {

                    currentNode = node;
                    shortestDist = estimatedDist;
                }
            }
        }
        
        row = currentNode->getRow();
        col = currentNode->getCol();
        distance = currentNode->getDistanceTraveled();

        if (env[row][col] == SYMBOL_GOAL) {
            nodesExplored->addElement(currentNode);
            reachedGoal = true;
        } else {

            // check for up position
            if (env[row - 1][col] != SYMBOL_WALL && !existsInList(row - 1, col, openList) ) {
            
                openList->addElement(new Node(row - 1,col,distance + 1));
            }

            // check for down position
            if (env[row + 1][col] != SYMBOL_WALL && !existsInList(row + 1, col, openList)) {
            
                openList->addElement(new Node(row + 1,col,distance + 1));
            }

            // check for left position
            if (env[row][col - 1] != SYMBOL_WALL && !existsInList(row, col - 1, openList)) {
            
                openList->addElement(new Node(row,col - 1,distance + 1));
            }

            // check for the right position
            if (env[row][col + 1] != SYMBOL_WALL && !existsInList(row, col + 1, openList)) {
            
                openList->addElement(new Node(row,col + 1,distance + 1));
            }   

            if (!existsInList(currentNode->getRow(), currentNode->getCol(), nodesExplored)) {
                nodesExplored->addElement(currentNode);
            }
        }

        count++;
 
    } while (!reachedGoal && currentNode != nullptr && count < 200);

    delete currentNode;
    delete goalNode;

}

NodeList* PathSolver::getNodesExplored(){

    return nodesExplored;
}

NodeList* PathSolver::getPath(Env env) {

    NodeList* shortestPath = new NodeList();
    Node* currentNode = nullptr;
    shortestPath->addElement(nodesExplored->getNode(nodesExplored->getLength()-1));
    bool reachedStart = false;

    int count = 1;
    do {

        currentNode = shortestPath->getNode(shortestPath->getLength()-1);

        //std::cout << "iteration " << count << ":" << currentNode->getRow() << "," << currentNode->getCol() << "," << currentNode->getDistanceTraveled() << std::endl;

        int row = currentNode->getRow();
        int col = currentNode->getCol();
        int distance = currentNode->getDistanceTraveled();

        if (env[row][col] == SYMBOL_START) {

            reachedStart = true;

        } else {
            
            // check for up position
            if (env[row - 1][col] != SYMBOL_WALL && existsInList(row - 1, col, nodesExplored) && !existsInList(row - 1, col, shortestPath) && getNodeByRowCol(row - 1, col, nodesExplored)->getDistanceTraveled() == distance - 1) {
                
                currentNode = getNodeByRowCol(row - 1, col, nodesExplored);
                shortestPath->addElement(currentNode);
            }

            // check for down position
            else if (env[row + 1][col] != SYMBOL_WALL && existsInList(row + 1, col, nodesExplored) && !existsInList(row + 1, col, shortestPath) && getNodeByRowCol(row + 1, col, nodesExplored)->getDistanceTraveled() == distance - 1) {
                
                currentNode = getNodeByRowCol(row + 1, col, nodesExplored);
                shortestPath->addElement(currentNode);
            }

            // check for left position
            else if (env[row][col - 1] != SYMBOL_WALL && existsInList(row, col - 1, nodesExplored) && !existsInList(row, col - 1, shortestPath) && getNodeByRowCol(row, col - 1, nodesExplored)->getDistanceTraveled() == distance - 1) {
            
                currentNode = getNodeByRowCol(row, col - 1, nodesExplored);
                shortestPath->addElement(currentNode);
            }

            // check for the right position
            else if (env[row][col + 1] != SYMBOL_WALL && existsInList(row, col + 1, nodesExplored) && !existsInList(row, col + 1, shortestPath) && getNodeByRowCol(row, col + 1, nodesExplored)->getDistanceTraveled() == distance - 1) {
                
                currentNode = getNodeByRowCol(row, col + 1, nodesExplored);
                shortestPath->addElement(currentNode);
            }   
        }

        // method added for testing and preventing infinite loop
        count++;
        
    } while (!reachedStart && currentNode != nullptr && count < 100);

    return shortestPath;
}

// function that checks for an adjacant path node that exists in the closed list (explored nodes list) and doesn't exist in the shortestPath list.
// used to check for shortest path back from Goal to Start.
bool PathSolver::validateByDirection(int row, int col, int distance, NodeList* nodeList, Env env) {

    if (env[row][col] != SYMBOL_WALL && existsInList(row, col, nodesExplored) && !existsInList(row, col, nodeList)) {

        Node* node = getNodeByRowCol(row, col, nodesExplored);

        if (node->getDistanceTraveled() == distance - 1) {

            return true;
        }
    } 

    return false;
}

// function that checks if a node at a particular row x col exists in a list.
bool PathSolver::existsInList(int row, int col, NodeList* nodeList) {

    for (int i = 0; i < nodeList->getLength(); i++) {

        if (row == nodeList->getNode(i)->getRow() && col == nodeList->getNode(i)->getCol()) {
            
            return true;
        }
    }
    return false;
}

// function to get a Node from a list based on a row and col values
Node* PathSolver::getNodeByRowCol(int row, int col, NodeList* nodeList) {

    for (int i = 0; i < nodeList->getLength(); i++) {
        if (nodeList->getNode(i)->getRow() == row && nodeList->getNode(i)->getCol() == col) {
            return nodeList->getNode(i);
        }
    }
    return nullptr;
}



//-----------------------------