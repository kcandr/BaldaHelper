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
#include <QPair>
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

void MainWindow::searchAllWords()
{
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            searchAllWordsFromCell(row, col);
        }
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
    text = new QTextEdit();
    rvl->addWidget(goButton);
    rvl->addWidget(text);
    rightFrame->setLayout(rvl);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(leftFrame);
    hl->addWidget(rightFrame);

    mainFrame->setLayout(hl);
    connect(goButton, SIGNAL(clicked()), SLOT(init()));
    setCentralWidget(mainFrame);
}

void MainWindow::searchAllWordsFromCell(int r, int c)
{
    QPair<int, int> tmp(r, c);
    visited.append(tmp);
    int co = getNeighbours(r, c);
    text->append(QString::number(co));
}

int MainWindow::getNeighbours(int r, int c)
{
    int count = 0;
    if ( (c < dimension - 1) &&
         ((characters[r][c + 1].type == neighbour) ||
          (characters[r][c + 1].type == chr)
          )
        ) {
        neighbours.append(QPair<int, int>(r, c + 1));
        ++count;
    }
    if ( (c > 0) &&
         ((characters[r][c - 1].type == neighbour) ||
          (characters[r][c - 1].type == chr)
          )
        ){
        neighbours.append(QPair<int, int>(r, c - 1));
        ++count;
    }
    if ( (r < dimension - 1) &&
         ((characters[r + 1][c].type == neighbour) ||
          (characters[r + 1][c].type == chr)
          )
        ){
        neighbours.append(QPair<int, int>(r + 1, c));
        ++count;
    }
    if ( (r > 0) &&
         ((characters[r - 1][c].type == neighbour) ||
          (characters[r - 1][c].type == chr)
          )
        ) {
        neighbours.append(QPair<int, int>(r - 1, c));
        ++count;
    }
    return count;
}

void MainWindow::init()
{
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            characters[row][col].type = lock;
            char tmp = *cells->item(row, col)->text().toAscii().data();
            characters[row][col].character = tmp;
            if (tmp) {
                characters[row][col].type = chr;
            }
        }
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            if (characters[row][col].character) {
                if ( (col < dimension - 1) && (characters[row][col + 1].type == lock) ) {
                    characters[row][col + 1].type = neighbour;
                    cells->item(row, col + 1)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (col > 0) && (characters[row][col - 1].type == lock) ) {
                    characters[row][col - 1].type = neighbour;
                    cells->item(row, col - 1)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (row < dimension - 1) && (characters[row + 1][col].type == lock) ) {
                    characters[row + 1][col].type = neighbour;
                    cells->item(row + 1, col)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (row > 0) && (characters[row - 1][col].type == lock) ) {
                    characters[row - 1][col].type = neighbour;
                    cells->item(row - 1, col)->setBackgroundColor(QColor("#CBE979"));
                }
            }
        }
    searchAllWordsFromCell(2, 4);
}

