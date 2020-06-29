/*
 * main.cpp
 *
 * Launch point for QApplication execution.
*/

#include "sortingalgapp.h"
#include "signaldata.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<signalData>("signalData");
    QCoreApplication::setOrganizationName("Diego's Engineering");
    QCoreApplication::setApplicationName("Sorting Algorithm App");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;

    /* provides the ability to define a set of options, parse the command-line
     * arguments, and store which options have actually been used, as well as
     *  option values.*/
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open");
    parser.process(app);

    SortingAlgApp w;
    w.show();
    return app.exec();
}
