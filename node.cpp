#include "node.h"
#include "edgebag.h"
#include <QString>

Node::Node() :
    lastIndex(0)
{
    data = new int[10];
    suffixLink = NULL;
    edges = new EdgeBag();
}

