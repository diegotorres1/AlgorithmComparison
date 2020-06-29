/*
 * sortingalgapp.cpp
 *
 * The MainWindow for the application. Grants users the ability to select
 * different sorting or search algorithms, run the algorithm and record
 * information about the runtime.
*/
#include "sortingalgapp.h"
#include "ui_sortingalgapp.h"
#include <iostream>
#include <QDebug>
#include "algorithms.h"
#include "signaldata.h"
#include "customtable.h"
#include <QButtonGroup>
#include <QCoreApplication>
#include "dialog.h"
#include <QSpinBox>
#include "ui_dialog.h"

SortingAlgApp::SortingAlgApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SortingAlgApp)
{
    qDebug() << "SortingAlgApp";
    ui->setupUi(this);
    /* Connect, Search and Sort QComboBoxes, Run and Input Data QPushButtons,
        and Table Size QSpinnerBox*/
    connect(ui->comboBox_alg, &QComboBox::currentTextChanged, this, [=]() {this->algSelection(*ui->comboBox_alg);});
    connect(ui->comboBox_algSearch, &QComboBox::currentTextChanged, this, [=]() {this->algSelection(*ui->comboBox_algSearch);});
    connect(ui->pushButton_run, &QPushButton::clicked, this, [=]() {this->runAlg(*ui->pushButton_run);});
    connect(ui->pushButton_inputData, &QPushButton::clicked, this, [=]() {this->openInputWindow();});
    connect(ui->spinBox_bufferSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]() {this->bufferChangeHandler();});
    ui->spinBox_bufferSize->setValue(10);

    /* Initialize custom table and stylize */
    this->table = new CustomTable(this->ui->customTableContainer);
    QGridLayout *layout = new QGridLayout(this->ui->customTableContainer);
    layout->addWidget(this->table);
    QMargins margins = layout->contentsMargins();
    layout->setContentsMargins(0, margins.top(), 0, margins.bottom());

    /* Algorithm to choose from and connection to run.*/
    alg = new Algorithms();
    alg->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, alg, &QObject::deleteLater); //What do do when the thread finishes
    connect(alg, &Algorithms::resultReady, this, &SortingAlgApp::handleResults);

    /* Connect Search and Sort QRadioButtons for user selections*/
    this->buttonGroup = new QButtonGroup();
    this->buttonGroup->addButton(ui->radioButton_sort,1);
    this->buttonGroup->addButton(ui->radioButton_search, 2);
    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ radioToggled();});
    ui->radioButton_sort->setChecked(true); // set default

    this->showMaximized();
}

void SortingAlgApp::bufferChangeHandler(){
    this->table->bufferSize = this->ui->spinBox_bufferSize->value();
}

void SortingAlgApp::openInputWindow(){
    // Do not reinitialize
    if(!(this->dialog)){
        this->dialog = new Dialog(this);
        this->dialog->show();
    }
    else{
        this->dialog->show();
    }

}

SortingAlgApp::~SortingAlgApp()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void SortingAlgApp::radioToggled(){
    qDebug() << "RADIO TOGGLED";
    QAbstractButton *button = this->buttonGroup->checkedButton();

    int id = this->buttonGroup->id(button);
    qDebug() << id ;
    // Obtain algorithm from user selection
    switch(id){
    case 1: // Sort
        this->algorithmName = this->ui->comboBox_alg->currentText();
        break;
    case 2: // Search
        this->algorithmName = this->ui->comboBox_algSearch->currentText();
        break;
    default:
        break;
    }
}

/*When the Alg Combobox is changed this function will be called*/
void SortingAlgApp::algSelection(QComboBox &comboBox){
    qDebug() << "Alg Selection\n";
    qDebug() << comboBox.currentText();
    this->algorithmName = QString(comboBox.currentText());
    return;

}

void SortingAlgApp::printArray(int arr[], int size)
{
    if(!arr){
        qDebug() << "Cannot print a null pointer.";
        return;
    }
    int i;
    std::string output = "";
    for (i = 0; i < size; i++)
         output += std::to_string(arr[i]) + ", ";
    qDebug() << output.c_str() << "\n";
}

void SortingAlgApp::runAlg(QPushButton &runButton){
    qDebug() << "Run clicked\n";

    qDebug() << algorithmName;
    int arr[] = {5,14,73,6,7,3,743,1};
    int *a;
    int high;
    int low = 0;
    int searchElement;

    /* If user data is provided grab otherwise use default values*/
    if(this->dialog && this->dialog->dataAvailable){
            searchElement = this->dialog->searchElement;
            qDebug() << "Data Available";
            a = this->dialog->arr;
            high = this->dialog->arrSize;

    }
    else{
        qDebug() << "Data Default";
        a = &arr[0];
        high = sizeof(arr) / sizeof(arr[0]);
        searchElement = 5;
    }

    signalData *data = new signalData(a);
    printArray(a, high);
    /*----SORTING----*/
    if(algorithmName.compare(QString("QuickSort"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->quickSort_init(data, low, high);}); //when operate signal is called do the work
    }
    else if(algorithmName.compare(QString("MergeSort"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->mergeSort_init(data, low, high);});
    }
    else if(algorithmName.compare(QString("BubbleSort"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->bubbleSort_init(data, high);});
    }
    else if(algorithmName.compare(QString("HeapSort"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->heapSort_init(data, high, 0);});
    }
    /*----SEARCHING----*/
    else if(algorithmName.compare(QString("LinearSearch"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->linearSearch_init(data, high, 5);});
    }
    else if(algorithmName.compare(QString("BinarySearch"),algorithmName) == 0){
        connect(this, &SortingAlgApp::operate, alg, [=]() {alg->binarySearch_init(data, high, 5);});
    }

    /* Start alg*/
    workerThread.start();
    emit operate();

    /*Remove connection to prevent double calls*/
    disconnect(this, 0, 0, 0);
    return;

}

void SortingAlgApp::handleResults(int time, signalData *data, int size, QString algName){
    qDebug() << "handleResults" ;
    qDebug() << time ;

    // Add to the outputs
    ui->label_value_totalTime->setText(QString(std::to_string(time).c_str()));
    ui->label_value_numElements->setText(QString(std::to_string(size).c_str()));

    /* Add the results to the table*/
    this->table->appendRowEntry(algName, false, time, size, std::time(nullptr));
    return;
}

void SortingAlgApp::dummy(){
    qDebug() << "Dummy\n";
}
