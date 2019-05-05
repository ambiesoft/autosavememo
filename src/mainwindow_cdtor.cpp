//#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>
#include <QStyle>
#include <QLabel>

//#include "../../lsMisc/stdQt/settings.h"
//#include "../../lsMisc/stdQt/stdQt.h"

#include "autosavememoapp.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

//using namespace AmbiesoftQt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    defaultFont_ = ui->plainTextEdit->font();

    QPalette palette = ui->plainTextEdit->palette();
    palette.setColor(QPalette::Highlight, QColor(Qt::GlobalColor::lightGray));
    palette.setColor(QPalette::HighlightedText, palette.text().color());
    ui->plainTextEdit->setPalette(palette);


    ui->action_New->setIcon(this->style()->standardIcon(QStyle::SP_FileIcon));
    ui->action_Open->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->action_Save->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->action_Undo->setIcon(this->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->action_Redo->setIcon(this->style()->standardIcon(QStyle::SP_MediaSeekForward));

    connect(ui->plainTextEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

    // menu
    connect(ui->menu_Character_Code, &QMenu::aboutToShow,
            this, &MainWindow::onCharacterCode_AboutToShow);


    ui->statusBar->addPermanentWidget(&statusAutomode_x);
    ui->statusBar->addPermanentWidget(&statusLabelCodec_);


#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    if (theApp->geometry().isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(theApp->geometry());
    }


    ui->action_Wrap->setChecked(theApp->wordWrap());
    on_action_Wrap_toggled(theApp->wordWrap());

    if(!theApp->fontString().isEmpty()) {
        QFont f;
        if(f.fromString(theApp->fontString()))
            ui->plainTextEdit->setFont(f);
    }


    setCurrentFile(QString(), QByteArray(), nullptr, false);
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
