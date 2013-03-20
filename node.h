#ifndef NODE_H
#define NODE_H

#include <QList>

class Edge;
class EdgeBag;

class Node
{
public:
    Node();
    QList<int> *getData();
    QList<int> *getData(int numElements);
    Node *getSuffixLink();
    void setSuffixLink(Node *suffix);
    void addEdge(char c, Edge *e);
    Edge *getEdge(char c);
    EdgeBag *getEdges();
    void addReference(int index);
    bool contains(int index);
    void addIndex(int index);
    int getResultCount();
    int computeAndCacheCount();
    int computeAndCacheCount(bool recursive);

private:
    QList<int> *data;
    int lastIndex;
    Node *suffixLink;
    EdgeBag *edges;
    int resultCount;
    static int START_SIZE;
    static int INCREMENT;

};

#endif // NODE_H
