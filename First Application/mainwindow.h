#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <stack>
#include "Collection.h"

class QLineEdit;
class QPushButton;
class QLabel;
class QListWidget;
class QComboBox;

class MainWindow : public QWidget
{
public:
    MainWindow();

private:
    Collection<int> intCollection;
    Collection<double> doubleCollection;
    Collection<string> stringCollection;

    QLineEdit* input;

    QPushButton* insertButton;
    QPushButton* removeButton;
    QPushButton* searchButton;
    QPushButton* sortButton;
    QPushButton* clearButton;
    QPushButton* clearInputButton;
    QPushButton* atButton;

    QPushButton* undoButton;
    QPushButton* redoButton;

    stack<Collection<int>> undoInt;
    stack<Collection<int>> redoInt;

    stack<Collection<double>> undoDouble;
    stack<Collection<double>> redoDouble;

    stack<Collection<string>> undoString;
    stack<Collection<string>> redoString;

    QListWidget* list;
    QLabel* status;
    QLabel* currentType;

    QPushButton* saveButton;
    QPushButton* loadButton;

    QLineEdit* indexInput;
    QLabel* count;

    QComboBox* typeBox;

    void updateList();
};

#endif