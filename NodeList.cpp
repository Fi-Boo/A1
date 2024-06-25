#include "NodeList.h"
#include <iostream>

NodeList::NodeList(){
    
    this->length = 0;
}

NodeList::~NodeList(){
    
    for (int i = 0; i < length; ++i) {
        delete nodes[i];
    }
}

NodeList::NodeList(NodeList& other){
    
    for (int i = 0; i < this->length; i++) {
        nodes[i] = new Node(*(other.nodes[i]));
    }
    this->length = other.length;
}

int NodeList::getLength(){
    
    return length;
}

void NodeList::addElement(Node* newPos){
    
    nodes[this->length] = newPos;
    this->length += 1;
}

Node* NodeList::getNode(int i){
    
    return nodes[i];
}

