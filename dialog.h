#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QMainWindow>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    QButtonGroup *buttonGroup_dataSelection; /*Button group for from file or generated*/
    QButtonGroup *buttonGroup_generateType; /*Button group for sequential or random generated*/
    QMainWindow *sortingalg; /**/


public:
    explicit Dialog(QWidget *parent = nullptr, QMainWindow *mainwindow = nullptr);
    ~Dialog();
    void setDefaultValues();
    /* Show user generated data*/
    QString setPreviewText(int arr[], int size);
    bool dataAvailable = false; /*FLAG if user data is available*/
    int *arr; /* user generated data*/
    int arrSize; /* size of user generated data*/
    int searchElement = 5; /* user generated searchElement*/

private:
    Ui::Dialog *ui;

private slots:
    /* Do Nothing */
    void radioToggledGenerateType(QAbstractButton *button);

    /* Handle when user chooses between Generate or From File data */
    void radioToggledDataSelection(QAbstractButton *button);

    /* Handler When user click generate data either sequeuntially/random*/
    void generateData();

    /* Handler open file is*/
    void openFile();
    void closeEvent(QCloseEvent *e = nullptr);
    void reject();
    void accept();

};

#endif // DIALOG_H
