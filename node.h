#ifndef NODE_H
#define NODE_H

class EdgeBag;

class Node
{
public:
    Node();

private:
    int *data;
    int lastIndex;
    Node *suffixLink;
    EdgeBag *edges;
    int resultCount;


};

#endif // NODE_H
