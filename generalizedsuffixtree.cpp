#include "generalizedsuffixtree.h"
#include "node.h"
#include "edge.h"
#include <QString>

GeneralizedSuffixTree::GeneralizedSuffixTree() :
    last(0)
{
    root = new Node();
    activeLeaf = root;
}

Node *GeneralizedSuffixTree::getRoot()
{
    return root;
}

void GeneralizedSuffixTree::put(const QString &key, int index)
{
    if (index < last) {
        return;
    } else {
        last = index;
    }

    activeLeaf = root;

    QString reminder = key;
    Node *s = root;

    QString text = "";

    for (int i = 0; i < reminder.length(); ++i) {
        text += reminder.at(i);
        // WARNING: need reminder.substring(i)
        QPair<Node *, QString> active = update(s, text, reminder.mid(i/*, reminder.length()*/), index);
        active = canonize(active.first, active.second);

        s = active.first;
        text = active.second;
    }

    if (NULL == activeLeaf->getSuffixLink() && activeLeaf != root && activeLeaf != s) {
        activeLeaf->setSuffixLink(s);
    }
}

int GeneralizedSuffixTree::computeCount()
{
    return root->computeAndCacheCount();
}

QList<int> *GeneralizedSuffixTree::search(QString word)
{
    return search(word, -1);
}

QList<int> *GeneralizedSuffixTree::search(QString word, int result)
{
    Node *tmpNode = searchNode(word);
    if (NULL == tmpNode) {
        return NULL;
    }
    return tmpNode->getData(result);
}

Node *GeneralizedSuffixTree::searchNode(QString word)
{
    Node *currentNode = root;
    Edge *currentEdge = NULL;

    for (int i = 0; i < word.length(); ++i) {
        char ch = word.at(i).toAscii();
        currentEdge = currentNode->getEdge(ch);
        if (NULL == currentEdge) {
            //there is no edge starting with this char
            return NULL;
        } else {
            QString label = currentEdge->getLabel();
            int lenToMatch = qMin(word.length() - i, label.length());
            if (!word.mid(i).contains(label.mid(0, lenToMatch))) {
                return NULL;
            }
            if (label.length() >= word.length() - i) {
                return currentEdge->getDestination();
            } else {
                currentNode = currentEdge->getDestination();
                i += lenToMatch - 1;
            }
        }
    }

    return NULL;
}

QPair<Node *, QString> GeneralizedSuffixTree::update(Node *inputNode, QString stringPath, QString rest, int value)
{
    Node *s = inputNode;
    QString tempString = stringPath;
    char newChar = stringPath.at(stringPath.length() - 1).toAscii();

    //line 1
    Node *oldRoot = root;
    QString part = tempString.mid(0, tempString.length() - 1);
    QPair<bool, Node *> ret = testAndSplit(s, part, newChar, rest, value);
    bool endPoint = ret.first;
    Node *r = ret.second;
    Node *leaf = NULL;

    //line 2
    while (!endPoint) {
        //line 3
        Edge *tempEdge = r->getEdge(newChar);
        if (NULL != tempEdge) {
            leaf = tempEdge->getDestination();
        } else {
            leaf = new Node();
            leaf->addReference(value);
            Edge *newEdge = new Edge(rest, leaf);
            r->addEdge(newChar, newEdge);
        }
        if (activeLeaf != root) {
            activeLeaf->setSuffixLink(leaf);
        }
        activeLeaf = leaf;

        //line 4
        if (oldRoot != root) {
            oldRoot->setSuffixLink(r);
        }
        //line 5
        oldRoot = r;
        //line 6
        if (NULL == s->getSuffixLink()) {
            //assert
            Q_ASSERT(root == s);
            tempString = tempString.mid(1);
        } else {
            QPair<Node *, QString> canret = canonize(s->getSuffixLink(), safeCutLastChar(tempString));
            s = canret.first;
            tempString = canret.second + tempString.at(tempString.length() - 1);
        }

        //line 7
        ret = testAndSplit(s, safeCutLastChar(tempString), newChar, rest, value);
        endPoint = ret.first;
        r = ret.second;
    }

    //line 8
    if (oldRoot != root) {
        oldRoot->setSuffixLink(r);
    }
    return *(new QPair<Node *, QString>(s, tempString));
}

QPair<bool, Node *> GeneralizedSuffixTree::testAndSplit(Node *inputs, QString stringPart, char t, QString remainder, int value)
{
    QPair<Node *, QString> ret = canonize(inputs, stringPart);
    Node *s = ret.first;
    QString str = ret.second;

    if (!str.isEmpty()) {
        Edge *g = s->getEdge(str.at(0).toAscii());

        QString label = g->getLabel();
        if (label.length() > str.length() && label.at(str.length()) == t) {
            return *(new QPair<bool, Node *>(true, s));
        } else {
            //split the edge
            QString newLabel = label.mid(str.length());
            //Q_ASSERT(label.startsWith(str));
            //build a new node and new edge
            Node *r = new Node();
            Edge *newEdge = new Edge(str, r);
            g->setLabel(newLabel);
            //link s->r
            r->addEdge(newLabel.at(0).toAscii(), g);
            s->addEdge(str.at(0).toAscii(), newEdge);

            return *(new QPair<bool, Node *>(false, r));
        }
    } else {
        Edge *e = s->getEdge(t);
        if (NULL == e) {
            return *(new QPair<bool, Node *>(false, s));
        } else {
            if (remainder == e->getLabel()) {
                e->getDestination()->addReference(value);
                return *(new QPair<bool, Node *>(true, s));
            } else if (remainder.startsWith(e->getLabel())) {
                return *(new QPair<bool, Node *>(true, s));
            } else if (e->getLabel().startsWith(remainder)) {
                //need to split as above
                Node *newNode = new Node();
                newNode->addReference(value);

                Edge *newEdge = new Edge(remainder, newNode);
                e->setLabel(e->getLabel().mid(remainder.length()));
                newNode->addEdge(e->getLabel().at(0).toAscii(), e);
                s->addEdge(t, newEdge);

                return *(new QPair<bool, Node *>(false, s));
            } else {
                //different words. No prefix.
                return *(new QPair<bool, Node *>(true, s));
            }
        }
    }
}

QPair<Node *, QString> GeneralizedSuffixTree::canonize(Node *s, const QString inputString)
{
    if (inputString.isEmpty()) {
        return *(new QPair<Node *, QString>(s, inputString));
    } else {
        Node *currentNode = s;
        QString string = inputString;
        Edge *g = s->getEdge(string.at(0).toAscii());

        while (g != NULL && string.startsWith(g->getLabel())) {
            string = string.mid(g->getLabel().length(), string.length());
            currentNode = g->getDestination();
            if (string.length() > 0) {
                g = currentNode->getEdge(string.at(0).toAscii());
            }
        }
        return *(new QPair<Node *, QString>(currentNode, string));
    }
}

QString GeneralizedSuffixTree::safeCutLastChar(const QString &seq)
{
    if (seq.length() == 0) {
        return "";
    }
    return seq.left(seq.length() - 1);
}

