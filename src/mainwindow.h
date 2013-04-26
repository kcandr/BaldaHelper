#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMultiHash>

class QListWidget;
class QStringList;
class QTableWidget;
class QTableWidgetItem;
class QPushButton;
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
    ~MainWindow();

private:
    int dimension;
    QStringList wordsList;
    GeneralizedSuffixTree* tree;
    QTableWidget* cells;
    QPushButton* goButton;
    QListWidget* words;
    QList<QPoint> newChar;
    QList<QPoint> existingChar;
    cellStructure characters[5][5];
    cellStructure fieldMask[5][5];
    QPoint fieldDirs[5][5];

    int readDictionary(QString fileName);
    void createTree();
    void createWindow();
    void findWords();
    void findWord(QPoint begin, QPoint end);

    bool inField(int r, int c);
    void goWater(int r, int c);
    void updateMask();
    void getWord(QPoint begin, QPoint end);
    void displayWords();
    void clearHighlightedWords();
    QMultiHash<QString, QList<QPoint> > wordsWayList;

private slots:
    void startFinder();
    void init();
    void highlightWord(QString text);
    void showWord(QString word);

};

#endif // MAINWINDOW_H
