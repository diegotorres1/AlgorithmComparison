#ifndef SORTINGALGAPP_H
#define SORTINGALGAPP_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QThread>
#include "signaldata.h"
#include "customtable.h"
#include "algorithms.h"
#include "dialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class SortingAlgApp; }
QT_END_NAMESPACE

class SortingAlgApp : public QMainWindow
{
    Q_OBJECT
    QThread workerThread; /* thread for alg*/
    CustomTable *table; /* output table for alg info*/
    Algorithms *alg; /* contains all alg definitions*/
    QButtonGroup *buttonGroup; /* button group for search/sort alg QRadioButton*/
    Dialog *dialog = nullptr; /* pop up window for user data input*/

public:
    SortingAlgApp(QWidget *parent = nullptr);
    ~SortingAlgApp();
    int *arr;

    QString algorithmName = QString("QuickSort"); /*Provide default algorithm*/
    static void printArray(int arr[], int size);


private:
    Ui::SortingAlgApp *ui;

private slots:
    /*Handler when an algorithm for either search or sorting alg is
        selected.*/
    void algSelection(QComboBox &comboBox);

    /*Handler when user presses run button to run alg.*/
    void runAlg(QPushButton &runButton);

    /*Handler when user selects either sort or search algorithm.*/
    void radioToggled();

    /*Handler when user clicks manually inputs data.*/
    void openInputWindow();

    /*Handler when user changes table buffer sizes.*/
    void bufferChangeHandler();

    /*dummy test for slots*/
    void dummy();

public slots:
    /*Handler when algorithm is done running and data needs to be posted
        to the results table.*/
    void handleResults(int time, signalData *data, int size, QString algName);

signals:
    /*Emitted when the user chooses to run alg.*/
    void operate();
};
#endif // SORTINGALGAPP_H
