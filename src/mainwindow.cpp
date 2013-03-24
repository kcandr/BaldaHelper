#include "mainwindow.h"
#include "generalizedsuffixtree.h"
#include "celldelegate.h"
#include <QtGui>
#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QTime>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dimension(5)
{
    tree = new GeneralizedSuffixTree();
    if (!readDictionary("Dictionary.txt")) {
        //createTree();
    }
    createWindow();
    setWindowTitle(tr("Balda Helper"));
}

MainWindow::~MainWindow()
{

}

int MainWindow::readDictionary(QString fileName)
{
    QFile dictionary(fileName);
    if (!dictionary.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", dictionary.errorString());
        return -1;
    }
    QTime t;
    t.start();

    while (!dictionary.atEnd()) {
        QString word = dictionary.readLine();
        wordsList.append(word.left(word.size() - 2));
    }
    qDebug("Time to reading dictionary: %d ms", t.elapsed());

    dictionary.close();
    return 0;
}

void MainWindow::createTree()
{
    qDebug("Adding to trie");
    QTime t;
    t.start();
    for (int i = 0; i < wordsList.size(); i++) {
        tree->put(wordsList.at(i), i);
    }
    qDebug("Time to creating tree: %d ms", t.elapsed());
    qDebug() << tree->search("карта")->at(0);
}

void MainWindow::createWindow()
{
    QFrame *mainFrame = new QFrame();

    QFrame *leftFrame = new QFrame();
    QVBoxLayout *lvl = new QVBoxLayout();
    cells = new QTableWidget();
    cells->setFixedSize(200, 200);
    cells->setRowCount(5);
    cells->verticalHeader()->setVisible(false);
    cells->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cells->setColumnCount(5);
    cells->horizontalHeader()->setVisible(false);
    cells->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cells->setItemDelegate(new CellDelegate());
    QFont font;
    font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
    cells->setFont(font);

    for (int row = 0; row < dimension; ++row) {
        for (int col = 0; col < dimension; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            item->setTextAlignment(Qt::AlignCenter);
            //item->setBackgroundColor(QColor(147, 185, 250));
            cells->setItem(row, col, item);
        }
        cells->setRowHeight(row, 40);
        cells->setColumnWidth(row, 40);
    }
    lvl->addWidget(cells);
    leftFrame->setLayout(lvl);

    QFrame *rightFrame = new QFrame();
    QVBoxLayout *rvl = new QVBoxLayout();
    goButton = new QPushButton(tr("GO"));
    rvl->addWidget(goButton);
    rightFrame->setLayout(rvl);
    //vl->addWidget(); add text widget

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(leftFrame);
    hl->addWidget(rightFrame);

    mainFrame->setLayout(hl);
    setCentralWidget(mainFrame);
}

