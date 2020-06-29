#include "signaldata.h"
#include <QDebug>
signalData::signalData()
{
    int x = 1;
}
signalData::signalData(int arr[])
{
    this->data = arr;
}


// destructor
signalData::~signalData(){
    qDebug() << "Destructor";
    delete data;
}
