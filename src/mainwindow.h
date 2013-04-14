#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
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
    lock,
    water
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
    QList<QPoint> visited;
    QList<QPoint> neighbours;
    QList<QPoint> newChar;
    QList<QPoint> existingChar;

    int readDictionary(QString fileName);
    void createTree();
    void createWindow();
    void searchAllWordsFromCell(int r, int c);
    int getNeighbours(int r, int c);
    void findWords();
    QString findWord(QPoint begin, QPoint end);

    bool inField(int r, int c);
    void goWater(int r, int c);
    void updateMask();
    QString getWord(QPoint begin, QPoint end);
    cellStructure fieldMask[5][5];
    QPoint fieldDirs[5][5];

private slots:
    void init();

};

#endif // MAINWINDOW_H
