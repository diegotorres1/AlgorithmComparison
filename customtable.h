#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QTableWidget>

class CustomTable : public QTableWidget
{
    Q_OBJECT
    int colCount;
public:
    explicit CustomTable(QWidget *parent = nullptr);
    void addColumnsLabels();
    void appendRowEntry(QString algName, bool threaded, float totalTime,  int size, time_t clock);
    void removeRowEntry(int rowIndex);
    int bufferSize = 10;

protected:
    void resizeEvent(QResizeEvent *event);

signals:

};

#endif // CUSTOMTABLE_H
