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

int readDictionary(QString fileName)
{
    QFile dictionary(fileName);
    if (!dictionary.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", dictionary.errorString());
        return -1;
    }
    QTime t;
    t.start();

    QString wordsList;
    while (!dictionary.atEnd()) {
        QString word = dictionary.readLine();
        qDebug("Size = %d ", word.size());
        wordsList.append(word);
    }
    qDebug("Size = %d ", wordsList.size());
    qDebug("Time to reading: %d ms", t.elapsed());

    dictionary.close();
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //int res = readDictionary("Dictionary.txt");
    GeneralizedSuffixTree *tree = new GeneralizedSuffixTree();
    tree->put("cacao", 0);
    qDebug() << tree->search("cac")->at(0);

    MainWindow w;
    w.show();
    
    return a.exec();
}
