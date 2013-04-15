#include "mainwindow.h"
#include "generalizedsuffixtree.h"
#include "celldelegate.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dimension(5)
{
    tree = new GeneralizedSuffixTree();
    if (!readDictionary("Dictionary_big.txt")) {
        createTree();
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
    words = new QListWidget();
    rvl->addWidget(goButton);
    rvl->addWidget(words);
    rightFrame->setLayout(rvl);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(leftFrame);
    hl->addWidget(rightFrame);

    mainFrame->setLayout(hl);
    connect(goButton, SIGNAL(clicked()), SLOT(startFinder()));
    connect(words, SIGNAL(currentTextChanged(QString)), SLOT(highlightWord(QString)));
    setCentralWidget(mainFrame);
}

void MainWindow::findWords()
{
    foreach (QPoint nChar, newChar) {
        foreach (QPoint eChar, existingChar) {
            updateMask();
            fieldMask[nChar.x()][nChar.y()].type = chr;
            findWord(nChar, eChar);
        }
    }
}

void MainWindow::findWord(QPoint begin, QPoint end)
{
    fieldMask[end.x()][end.y()].type = water;
    for (int counter = 0; counter < 50; ++counter) {
        for (int r = 0; r < dimension; ++r)
            for (int c = 0; c < dimension; ++c) {
                if (fieldMask[r][c].type == water) {
                    goWater(r, c);
                }
            }
        if (fieldMask[begin.x()][begin.y()].type == water) {
            getWord(begin, end);
            return;
        }
    }
}

void MainWindow::getWord(QPoint begin, QPoint end)
{
    QPoint p1 = begin;
    QPoint p2;
    QString word(fieldMask[p1.x()][p1.y()].character);
    QList<QPoint> pointWay;
    pointWay.append(p1);

    while (true) {
        p2 = fieldDirs[p1.x()][p1.y()];

        word.append(fieldMask[p2.x()][p2.y()].character);
        pointWay.append(p2);
        p1 = p2;

        if (p1 == end) {
            break;
        }
    }
    wordsWayList.insert(word, pointWay);
}

void MainWindow::displayWords()
{
    QMultiHash<QString, QList<QPoint> >::iterator it = wordsWayList.begin();
    words->addItem(it.key());
    QMultiHash<QString, QList<QPoint> >::iterator prev = it;
    ++it;

    while (it != wordsWayList.end()) {
        if (prev.key() != it.key()) {
            words->addItem(it.key());
            prev = it;
        }
        ++it;
    }
}

void MainWindow::highlightWord(QString text)
{
    clearHighlightedWords();
    QMultiHash<QString, QList<QPoint> >::iterator it = wordsWayList.find(text);
    QList<QList<QPoint> > values = wordsWayList.values(it.key());

    foreach (QList<QPoint> lop, values) {
        foreach (QPoint p, lop) {
            cells->item(p.x(), p.y())->setBackgroundColor(QColor("#E165B9"));
        }
    }
}

void MainWindow::clearHighlightedWords()
{
    cells->clearSelection();
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            cells->item(row, col)->setBackgroundColor(QColor(1, 0, 0, 0));
        }
}

void MainWindow::startFinder()
{
    init();
    findWords();
    displayWords();

    qDebug() << tree->search("карта")->at(0);
    qDebug() << tree->search("парта")->at(0);
}

bool MainWindow::inField(int r, int c)
{
    if (r >= 0 && r < dimension &&
        c >= 0 && c < dimension) {
        return true;
    }
    return false;
}

void MainWindow::goWater(int r, int c)
{
    // up
    if (inField(r - 1, c) && fieldMask[r - 1][c].type == chr) {
        fieldMask[r - 1][c].type = water;
        fieldDirs[r - 1][c].setX(r);
        fieldDirs[r - 1][c].setY(c);
    }
    // left
    if (inField(r , c - 1) && fieldMask[r][c - 1].type == chr) {
        fieldMask[r][c - 1].type = water;
        fieldDirs[r][c - 1].setX(r);
        fieldDirs[r][c - 1].setY(c);
    }
    // down
    if (inField(r + 1, c) && fieldMask[r + 1][c].type == chr) {
        fieldMask[r + 1][c].type = water;
        fieldDirs[r + 1][c].setX(r);
        fieldDirs[r + 1][c].setY(c);
    }
    // right
    if (inField(r , c + 1) && fieldMask[r][c + 1].type == chr) {
        fieldMask[r][c + 1].type = water;
        fieldDirs[r][c + 1].setX(r);
        fieldDirs[r][c + 1].setY(c);
    }
}

void MainWindow::updateMask()
{
    for (int r = 0; r < dimension; ++r)
        for (int c = 0; c < dimension; ++c) {
            fieldMask[r][c] = characters[r][c];
            fieldDirs[r][c] = QPoint();
        }
}

void MainWindow::init()
{
    words->clear();
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            characters[row][col].type = lock;
            char tmp = *cells->item(row, col)->text().toAscii().data();
            characters[row][col].character = tmp;
            if (tmp) {
                characters[row][col].type = chr;
                existingChar.append(QPoint(row, col));
            }
        }

    QString tmp("_");
    for (int row = 0; row < dimension; ++row)
        for (int col = 0; col < dimension; ++col) {
            if (characters[row][col].type == chr) {
                if ( (col < dimension - 1) && (characters[row][col + 1].type == lock) ) {
                    characters[row][col + 1].type = neighbour;
                    characters[row][col + 1].character = *tmp.toAscii().data();
                    newChar.append(QPoint(row, col + 1));
                    cells->item(row, col + 1)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (col > 0) && (characters[row][col - 1].type == lock) ) {
                    characters[row][col - 1].type = neighbour;
                    characters[row][col - 1].character = *tmp.toAscii().data();
                    newChar.append(QPoint(row, col - 1));
                    cells->item(row, col - 1)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (row < dimension - 1) && (characters[row + 1][col].type == lock) ) {
                    characters[row + 1][col].type = neighbour;
                    characters[row + 1][col].character = *tmp.toAscii().data();
                    newChar.append(QPoint(row + 1, col));
                    cells->item(row + 1, col)->setBackgroundColor(QColor("#CBE979"));
                }
                if ( (row > 0) && (characters[row - 1][col].type == lock) ) {
                    characters[row - 1][col].type = neighbour;
                    characters[row - 1][col].character = *tmp.toAscii().data();
                    newChar.append(QPoint(row - 1, col));
                    cells->item(row - 1, col)->setBackgroundColor(QColor("#CBE979"));
                }
            }
        }
}


