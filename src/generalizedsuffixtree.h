#ifndef GENERALIZEDSUFFIXTREE_H
#define GENERALIZEDSUFFIXTREE_H

#include <QPair>
#include <QList>

class Node;
class QString;

class GeneralizedSuffixTree
{
public:
    GeneralizedSuffixTree();
    Node *getRoot();
    void put(const QString &key, int index);
    int computeCount();
    QList<int> *search(QString word);
    QList<int> *search(QString word, int result);

private:
    int last;
    Node *root;
    Node *activeLeaf;

    QPair<Node *, QString> update(Node *inputNode, QString stringPath, QString rest, int value);
    QPair<bool, Node *> testAndSplit(Node *inputs, QString stringPart, char t, QString remainder, int value);
    QPair<Node *, QString> canonize(Node *s, const QString inputString);
    QString safeCutLastChar(const QString &seq);
    Node *searchNode(QString word);
};

#endif // GENERALIZEDSUFFIXTREE_H
