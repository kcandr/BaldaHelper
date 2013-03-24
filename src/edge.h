#ifndef EDGE_H
#define EDGE_H

#include <QString>

class Node;

class Edge
{
public:
    Edge();
    Edge(const Edge &edge);
    Edge &operator=(const Edge &other);
    Edge(QString label, Node *destinationNode);
    QString getLabel();
    void setLabel(QString newLabel);
    Node *getDestination();
    void setDestination(Node *newDestination);

private:
    QString label;
    Node *destinationNode;

};

#endif // EDGE_H
