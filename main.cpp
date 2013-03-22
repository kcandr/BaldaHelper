#include "mainwindow.h"
#include <QFile>
#include <QTime>
#include <QTextCodec>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include "generalizedsuffixtree.h"

int readDictionary(QString fileName, GeneralizedSuffixTree *gTree)
{
    QFile dictionary(fileName);
    if (!dictionary.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", dictionary.errorString());
        return -1;
    }
    QTime t;
    t.start();

    QStringList wordsList;
    int i = 0;
    while (!dictionary.atEnd()) {
        QString word = dictionary.readLine();
        word = word.left(word.size() - 2);
        //wordsList.append(word);
        gTree->put(word, i++);
        //qDebug("Size = %d ", word.size());
        //wordsList.append(word);
    }
    //qDebug("Size = %d ", wordsList.size());
    qDebug("Time to reading: %d ms", t.elapsed());

    dictionary.close();
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    //int res = readDictionary("Dictionary.txt");
    GeneralizedSuffixTree *tree = new GeneralizedSuffixTree();
    int res = readDictionary("Dictionary.txt", tree);

//    QFile dictionary("D.txt");
//    dictionary.open(QIODevice::ReadOnly);
//    int i = 0;
//    while (!dictionary.atEnd()) {
//        QString str = dictionary.readLine();
//        str = str.left(str.size() - 2);
//        tree->put(str, i++);
//    }
//    dictionary.close();
    qDebug() << tree->search("карта")->at(0);

    MainWindow w;
    w.show();

    return a.exec();
}
