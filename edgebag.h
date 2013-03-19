#ifndef EDGEBAG_H
#define EDGEBAG_H

#include <QHash>
#include <QList>

class QByteArray;
class Edge;

class EdgeBag
{
public:
    EdgeBag();
    void put(char c, Edge e);
    Edge *get(char c);
    int search(char c);
    QList<Edge *> getValues();
    void sortArrays();

private:
    QHash<char, Edge *> *children;
};

#endif // EDGEBAG_H
