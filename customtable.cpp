#include "customtable.h"
#include <QLabel>
#include <QResizeEvent>
#include <QHeaderView>
#include <ctime>
#include <time.h>
CustomTable::CustomTable(QWidget *parent) : QTableWidget(parent)
{
    addColumnsLabels();
    this->colCount = 5;
}

void CustomTable::addColumnsLabels(){

    this->insertColumn(0); // Sorting Alg Name
    this->insertColumn(1); // Threaded Yes/No
    this->insertColumn(2); // Total Time
    this->insertColumn(3); // Size
    this->insertColumn(4); // Time Stamp
    this->setHorizontalHeaderLabels(QStringList({"Alg Name", "Threaded", "Total Time", "Size", "Clock"}));
    this->horizontalHeader()->setStretchLastSection(true);

}

void CustomTable::resizeEvent(QResizeEvent *event){

    int w = this->width()/this->colCount * .9;
    int i;
    for(i = 0; i < this->colCount; i++)
        this->setColumnWidth(i, w);

    event->accept();
}

void CustomTable::appendRowEntry(QString algName, bool threaded, float totalTime, int size, time_t clock){
    int rowIndex = this->rowCount(); //insert and empty row
    this->insertRow(rowIndex);

    /* Fill the row with data */
    this->setCellWidget(rowIndex, 0, new QLabel(algName));
    this->setCellWidget(rowIndex, 1, threaded ? new QLabel("Yes"):new QLabel("No"));
    QString *time = new QString(std::to_string(totalTime).c_str());
    this->setCellWidget(rowIndex, 2, new QLabel(*time));
    this->setCellWidget(rowIndex, 3, new QLabel(std::to_string(size).c_str()));

    /* Add the time*/
    struct tm * timeinfo;
    char buffer [80];
    timeinfo = localtime (&clock);
    strftime (buffer,80,"%I:%M%p.",timeinfo);
    this->setCellWidget(rowIndex, 4, new QLabel(QString(buffer)));

    /* Prevent table row overflow*/
    if(rowIndex > this->bufferSize){
        for(int i = 0; i < rowIndex - this->bufferSize; i++){
            this->removeRowEntry(0);
        }
    }
    if(rowIndex==this->bufferSize){
        this->removeRowEntry(0);
    }

}
void CustomTable::removeRowEntry(int rowIndex){
    this->removeRow(rowIndex);
    return;
}


