#include "edge.h"
#include "node.h"

Edge::Edge() :
    label(""), destinationNode(NULL)
{
}

Edge::Edge(const Edge &edge)
{
    this->label = edge.label;
    this->destinationNode = edge.destinationNode;
}

Edge &Edge::operator =(const Edge &other)
{
    this->label = other.label;
    this->destinationNode = other.destinationNode;
    return *this;
}

Edge::Edge(QString label, Node *destinationNode) :
    label(label), destinationNode(destinationNode)
{
}

QString Edge::getLabel()
{
    return label;
}

void Edge::setLabel(QString newLabel)
{
    this->label = newLabel;
}

Node *Edge::getDestination()
{
    return destinationNode;
}

void Edge::setDestination(Node *newDestination)
{
    this->destinationNode = newDestination;
}
