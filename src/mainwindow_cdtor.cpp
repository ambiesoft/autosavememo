//#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>
#include <QStyle>
#include <QLabel>

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

    ui->action_New->setIcon(this->style()->standardIcon(QStyle::SP_FileIcon));
    ui->action_Open->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->action_Save->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->plainTextEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);


    ui->statusBar->addPermanentWidget(&statusAutomode_x);
    ui->statusBar->addPermanentWidget(&statusLabelCodec_);


#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    // Read from ini
    readSettings();

    setCurrentFile(QString(), QByteArray(), nullptr, false);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
