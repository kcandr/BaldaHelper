#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class QStringList;
class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QTextEdit;
class GeneralizedSuffixTree;

enum {
    chr,
    neighbour,
    lock
};

struct cellStructure {
    int type;
    char character;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void searchAllWords();
    ~MainWindow();

private:
    int dimension;
    QStringList wordsList;
    GeneralizedSuffixTree* tree;
    QTableWidget* cells;
    QPushButton* goButton;
    QTextEdit* text;
    cellStructure characters[5][5];
    QList<QPair<int, int> > visited;
    QList<QPair<int, int> > neighbours;

    int readDictionary(QString fileName);
    void createTree();
    void createWindow();
    void searchAllWordsFromCell(int r, int c);
    int getNeighbours(int r, int c);

private slots:
    void init();

};

#endif // MAINWINDOW_H
