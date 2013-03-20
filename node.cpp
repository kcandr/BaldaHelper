#include "node.h"
#include "edge.h"
#include "edgebag.h"
#include <QString>

int Node::START_SIZE = 0;
int Node::INCREMENT = 1;

Node::Node() :
    lastIndex(0), resultCount(-1)
{
    data = new QList<int>();
    suffixLink = NULL;
    edges = new EdgeBag();
}

QList<int> *Node::getData()
{
    return getData(-1);
}

QList<int> *Node::getData(int numElements)
{
    QList<int> *ret = new QList<int>();
    foreach (int num, *data) {
        ret->append(num);
        if (ret->size() == numElements) {
            return ret;
        }
    }

    int missingItems = numElements == -1 ? -1 : numElements - ret->size();
    foreach (Edge *e, edges->getValues()) {
        if (-1 == numElements || ret->size() < numElements) {
            foreach (int num, *e->getDestination()->getData(missingItems)) {
                ret->append(num);
                if (ret->size() == numElements) {
                    return ret;
                }
            }
            missingItems = numElements == -1 ? -1 : numElements - ret->size();
        }
    }
    return ret;
}

Node *Node::getSuffixLink()
{
    return suffixLink;
}

void Node::setSuffixLink(Node *suffix)
{
    this->suffixLink = suffix;
}

void Node::addEdge(char c, Edge *e)
{
    edges->put(c, e);
}

Edge *Node::getEdge(char c)
{
    Edge *tmp = edges->get(c);
    return tmp;
}

EdgeBag *Node::getEdges()
{
    return edges;
}

void Node::addReference(int index)
{
    if (contains(index)) {
        return;
    }

    addIndex(index);
    Node *iter = this->suffixLink;
    while (iter != NULL) {
        if (iter->contains(index)) {
            return;
        }
        iter->addReference(index);
        iter = iter->getSuffixLink();
    }
}

bool Node::contains(int index)
{
    int low = 0;
    int high = lastIndex - 1;

    while (low <= high) {
        int mid = (low + high) >> 1;
        int midValue = data->at(mid);

        if (midValue < index) {
            low = mid + 1;
        } else if (midValue > index) {
            high = mid - 1;
        } else {
            return true;
        }
    }
    return false;
}

void Node::addIndex(int index)
{
    data->append(index);
    ++lastIndex;
}

int Node::getResultCount()
{
    return resultCount;
}

int Node::computeAndCacheCount()
{
    return computeAndCacheCount(true);
}

int Node::computeAndCacheCount(bool recursive)
{
    resultCount = getData()->size();

    if (recursive) {
        foreach (Edge *e, edges->getValues()) {
            e->getDestination()->computeAndCacheCount();
        }
    }

    return resultCount;
}
