#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class QStringList;
class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class GeneralizedSuffixTree;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    int dimension;
    QStringList wordsList;
    GeneralizedSuffixTree *tree;
    QTableWidget *cells;
    QPushButton *goButton;

    int readDictionary(QString fileName);
    void createTree();
    void createWindow();

private slots:
    void setCellsBackground(int row, int col);
};

#endif // MAINWINDOW_H
