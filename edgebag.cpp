#include "edgebag.h"
#include "edge.h"
#include <QHash>
#include <QList>

EdgeBag::EdgeBag()
{
    children = new QHash<char, Edge *>();
}
// NOTE: check search() needed realy?
void EdgeBag::put(char c, Edge *e)
{
//    if (c != (char)(BYTE) c) {
//        return;
//    }
    if (children == NULL) {
        children = new QHash<char, Edge *>();
    }
    int index = search(c);

    if (index < 0) {
        //
        children->insert(c, e);
    } else {
        //values[index] = e;
        children->find(c).value() = e;
        //values->replace(index, &e);
    }
}

Edge *EdgeBag::get(char c)
{
    //
    int index = search(c);
    if (index < 0) {
        return NULL;
    }
    return children->find(c).value();
    //return values->at(index);
}


int EdgeBag::search(char c)
{
    if (children == NULL) {
        return -1;
    }

    int i = 0;
    for (QHash<char, Edge *>::Iterator it = children->begin(); it != children->end(); ++it, ++i) {
        if (c == it.key()) {
            return i;
        }
    }
    return -1;
}

QList<Edge *> EdgeBag::getValues()
{
    if (children == NULL) {
        QList<Edge *> tmp;
        return tmp;
    } else {
        return children->values();
    }
}
