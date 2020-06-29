#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QObject>
#include <QThread>
#include "signaldata.h"

class Algorithms : public QObject
{
    Q_OBJECT
public:
    Algorithms();
    // QuickSort
    static void swap(int *a, int *b);
    static int partition(int arr[], int low, int high);

    // MergeSort
    static void merge(int arr[], int l, int m, int r);
    void heapify(int arr[], int size, int index);



public slots:
    void quickSort(int arr[], int low, int high);
    void quickSort_init(signalData *data, int low, int high);

    void mergeSort(int arr[], int l, int r);
    void mergeSort_init(signalData *data, int l, int r);


    void bubbleSort(int arr[], int size);
    void bubbleSort_init(signalData *data, int size);

    void heapSort(int arr[], int size, int index);
    void heapSort_init(signalData *data, int size, int index);

    int linearSearch(int arr[], int size, int num);
    void linearSearch_init(signalData *data, int size, int num = 10);

    int binarySearch(int arr[], int size, int num, int low, int high);
    void binarySearch_init(signalData * data, int size, int num);

signals:
    void resultReady(int time, signalData *data, int size, QString algName);
};

#endif // ALGORITHMS_H
