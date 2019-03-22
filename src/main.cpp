#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QSharedPointer>
#include <QVector>

#include "../../lsMisc/stdQt/stdQt.h"

#include "autosavememoapp.h"
#include "consts.h"

using namespace Consts;
using namespace AmbiesoftQt;

void processCommandLine(QCoreApplication& app, QStringList& files)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
//    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
//    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "file to open."));

    // A boolean option with a single name (-p)
//    QCommandLineOption showProgressOption("p", QCoreApplication::translate("main", "Show progress during copy"));
//    parser.addOption(showProgressOption);

    // A boolean option with multiple names (-f, --force)
//    QCommandLineOption forceOption(QStringList() << "f" << "force",
//                                   QCoreApplication::translate("main", "Overwrite existing files."));
//    parser.addOption(forceOption);

    // An option with a value
//    QCommandLineOption targetDirectoryOption(QStringList() << "t" << "target-directory",
//                                             QCoreApplication::translate("main", "Copy all source files into <directory>."),
//                                             QCoreApplication::translate("main", "directory"));
//    parser.addOption(targetDirectoryOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    files = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

//    bool showProgress = parser.isSet(showProgressOption);
//    bool force = parser.isSet(forceOption);
//    QString targetDir = parser.value(targetDirectoryOption);
}

void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    Q_UNUSED(msg);
}

int main(int argc, char *argv[])
{
    AutosavememoApp app(argc, argv);
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setApplicationName(APPNAME);
    QCoreApplication::setApplicationVersion(APPVER);

#ifdef QT_NO_DEBUG
    if ( !app.arguments().contains(QLatin1String("--with-debug") ))
    {
        qInstallMessageHandler(noMessageOutput);
    }
#endif

    if(!app.readSettings())
    {
        Alert(nullptr,
              QObject::tr("Failed to read setting."));
        return 1;
    }

    QStringList filesToOpen;
    processCommandLine(app, filesToOpen);

    QVector<QSharedPointer<MainWindow>> wins;

    if(filesToOpen.isEmpty())
    {
        wins.append(QSharedPointer<MainWindow>(new MainWindow()));
    }
    else
    {
        for(auto&& file: filesToOpen)
        {
            QSharedPointer<MainWindow> pWin(new MainWindow);
            pWin->loadFile(file);
            wins.append(pWin);
        }
    }
//    MainWindow w1;
//    MainWindow w2;

//    w1.loadFile(filesToOpen[0]);
//    w2.loadFile(filesToOpen[1]);

//    w1.show();
//    w2.show();

    for(auto&& win : wins)
    {
        win->show();
    }

    int ret = app.exec();

    if(!app.writeSettings())
    {
        Alert(nullptr,
              QObject::tr("Failed to write settings."));
        return 1;
    }
    return ret;
}
