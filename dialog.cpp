#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include "sortingalgapp.h"
#include <QMessageBox>
#include <QStringBuilder>
#include <QFile>
#include <QFileDialog>
#include <QCloseEvent>
#include "sortingalgapp.h"

Dialog::Dialog(QWidget *parent, QMainWindow *mainwindow) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    setWindowTitle(QString("Data Selection"));
    // Default select generate option

    // Create Button Groups
    this->buttonGroup_dataSelection = new QButtonGroup(); // generate vs from file
    this->buttonGroup_generateType = new QButtonGroup(); // sequential vs random

    this->buttonGroup_dataSelection->addButton(this->ui->radioButton_fromFile);
    this->buttonGroup_dataSelection->addButton(this->ui->radioButton_generate);
    this->buttonGroup_generateType->addButton(this->ui->radioButton_sequential);
    this->buttonGroup_generateType->addButton(this->ui->radioButton_random);

    // Connect Button Groups to handlers
    connect(this->buttonGroup_dataSelection, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ radioToggledDataSelection(button);});

    connect(this->buttonGroup_generateType, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ radioToggledGenerateType(button);});

    // Set the default button checks
    this->ui->radioButton_generate->setChecked(true);
    radioToggledDataSelection(this->ui->radioButton_generate);
    this->ui->radioButton_sequential->setChecked(true);
    radioToggledDataSelection(this->ui->radioButton_sequential);

    this->setDefaultValues();

    // Misc Connections
    connect(ui->pushButton_generate, &QPushButton::clicked, this, [=]() {this->generateData();});
    connect(ui->pushButton_openFile, &QPushButton::clicked, this, [=]() {this->openFile();});
    connect(ui->spinBox_searchElement,QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int i) {this->searchElement = i;});

    this->sortingalg = mainwindow;
}

QString Dialog::setPreviewText(int arr[], int size){
    QString previewstr("");
    int i;

    previewstr = previewstr % QString("[");
    for(i = 0; i<size;i++){
        previewstr = previewstr % QString(std::to_string(arr[i]).c_str()) % QString(", ");
    }
    previewstr = previewstr % QString("]");

    this->ui->textBrowser->setText(previewstr);
    this->arr = arr;
    return previewstr;
}

// Generate Data handler
void Dialog::generateData(){
    this->ui->textBrowser->setText(QString(""));
    this->dataAvailable = false;
    bool sequential;
    int upperLimit, lowerLimit, numElements, searchNum;

    QString generateType = buttonGroup_generateType->checkedButton()->text();
    sequential = generateType.compare("Sequential") == 0 ? true : false;

    upperLimit = this->ui->spinBox_upperLimit->value();
    lowerLimit = this->ui->spinBox_lowerLimit->value();
    numElements = this->ui->spinBox_numElements->value();
    searchNum = this->ui->spinBox_searchElement->value();
    // Check limits
    if(upperLimit <= lowerLimit){
        QMessageBox msgBox;
        msgBox.setText("Error : Upper Limit must > Lower Limit");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    int *arr = new int[numElements];

    // inclusive lower and upper
    auto sequentialArrLambda = [](int arr[], int lower, int upper, int size)
    {
        int index, lowerValue;
        lowerValue = lower;
        for(index = 0;index < size; index++){
            if(lowerValue <= upper){
                arr[index] = lowerValue;

            }
            else if(lowerValue > upper){
                lowerValue = lower;
                arr[index] = lowerValue;
            }
            lowerValue++;

        }
        return;
    };
    auto randomArrLambda = [](int arr[], int lower, int upper, int size)
    {
        int index, randLower, randUpper;
        randUpper = upper;
        randLower = lower;

         // Logic is wrong, keep producing sequential if size is larger than upper
        for(index = 0; index < size; index++){
            arr[index] = (rand() % (randUpper - randLower)) + randLower;
        }
        return;
    };

    if(sequential)
        sequentialArrLambda(arr, lowerLimit, upperLimit, numElements);
    else
        randomArrLambda(arr, lowerLimit, upperLimit, numElements);

    qDebug() << "lower : " << lowerLimit << ", upper : " << upperLimit << ", size : " << numElements;
    SortingAlgApp::printArray(arr, numElements);
    setPreviewText(arr,numElements);
    this->dataAvailable = true;
    arrSize = numElements;
    this->arr = &arr[0];
    SortingAlgApp::printArray(this->arr, numElements);
    return;
}

// When data selection is toggled change the page
void Dialog::radioToggledDataSelection(QAbstractButton *button){
    button = this->buttonGroup_dataSelection->checkedButton();
    QString label = button->text();
    if(label.compare(QString("Generate")) == 0){
        qDebug() << "Generate";
        this->ui->fromFileContainer->hide();
        this->ui->generateContainer->show();
    }
    else if(label.compare(QString("From File")) == 0){
        qDebug() << "From File";
        this->ui->fromFileContainer->show();
        this->ui->generateContainer->hide();
    }

    return;
}

void Dialog::radioToggledGenerateType(QAbstractButton *button){
    return;
}

void Dialog::setDefaultValues(){
    this->ui->spinBox_lowerLimit->setValue(0);
    this->ui->spinBox_upperLimit->setValue(200);
    this->ui->spinBox_numElements->setValue(100);
    this->ui->spinBox_searchElement->setValue(5);


}

void Dialog::openFile(){
    this->ui->textBrowser->setText(QString(""));
    int numLines;
    this->dataAvailable = false;
    // Open up a file open dialog window
    QFileDialog dialog(this);
//    dialog.setFileMode(QFileDialog::);
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "/home", tr("CSV Files (*.csv)"));

    auto errorPromptLambda = [](QString message)
    {
        QMessageBox msgBox;
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    };

    QFile file(fileName);
        // Need to handle more errors and provide error dialog
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << file.errorString();
            errorPromptLambda(QString("ERROR : Could not read the file"));
            return ;
        }

//        QStringList wordList;
        std::vector<int> arrVector;
        QList<QByteArray> byteArrayList;

        numLines = 0;
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
//            wordList.append(line.split(',').first());
            byteArrayList = line.split(',');
            numLines++;
        }
        if(numLines != 1){
            errorPromptLambda(QString("Only 1D csv required. 1 Row only."));
            return;
        }
        // iterate through byte arrays
        for (int i = 0; i < byteArrayList.size(); ++i) {
            arrVector.push_back(byteArrayList.at(i).toInt());
        }
        int *arrArray = new int[arrVector.size()];
        for(int i = 0; i < arrVector.size();i++){
            arrArray[i] = arrVector.at(i);
        }

        setPreviewText(arrArray, arrVector.size());
        this->ui->label_fileoutput->setText(fileName);
    this->arr = arrArray;
    arrSize = arrVector.size();
    this->dataAvailable = true;
    return;
}

void Dialog::reject()
{
    qDebug()<<"Cancel";
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    if (true) {
        resBtn = QMessageBox::question( this, "name",
                                        tr("Are you sure?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
    }
    if (resBtn == QMessageBox::Yes) {
        QDialog::reject();
    }
}

void Dialog::accept(){
    qDebug() << "Pressed Ok";
    closeEvent();
    SortingAlgApp::printArray(arr, arrSize);
}

void Dialog::closeEvent(QCloseEvent *e){
    qDebug() << "Close Event ";
    this->hide();

}


Dialog::~Dialog()
{
    delete ui;
}
