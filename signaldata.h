#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QMetaType>
class signalData
{

public:
    int *data;
    signalData();
    signalData(int arr[]);
    ~signalData();

};
Q_DECLARE_METATYPE(signalData);
#endif // SIGNALDATA_H
