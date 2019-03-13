//#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>

//#include "../../lsMisc/stdQt/settings.h"
//#include "../../lsMisc/stdQt/stdQt.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

//using namespace AmbiesoftQt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->plainTextEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);


#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    // Read from ini
    readSettings();

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
