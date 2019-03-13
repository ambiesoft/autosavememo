#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QDir>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QSessionManager>
#include <QStandardPaths>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();

    connect(ui->plainTextEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);


#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}
bool MainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
        if(ui->plainTextEdit->document()->isModified())
        {
            if(!curFile.isEmpty())
            {
                on_action_Save_triggered();
            }
        }
    }
    return ParentClass::event(e);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_plainTextEdit_textChanged()
{
    // setWindowModified(ui->plainTextEdit->document()->isModified());
}
void MainWindow::documentWasModified()
{
    updateTitle();
    setWindowModified(ui->plainTextEdit->document()->isModified());
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}


bool MainWindow::maybeSave()
{
    if (!ui->plainTextEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return on_action_Save_triggered();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::readSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    fileDirectory_ = settings.valueString("fileDirectory");
    if(fileDirectory_.isEmpty())
        fileDirectory_ = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}
void MainWindow::writeSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());

    settings.setValue("fileDirectory", fileDirectory_);
}

bool MainWindow::on_action_Save_triggered()
{
    if (curFile.isEmpty()) {
        return on_actionSave_As_triggered();
    } else {
        return saveFile(curFile);
    }
}


bool MainWindow::on_actionSave_As_triggered()
{
    QFileDialog dialog(this);
    dialog.setDirectory(fileDirectory_);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    QString file = dialog.selectedFiles().first();
    fileDirectory_ = QFileInfo(file).path();
    return saveFile(file);
}

bool MainWindow::saveFile(const QString &strFileName)
{
    QString strNewTmpFile = (strFileName+".saving");
    QString strBackupFile = (strFileName+".bkmemo");

    {
        QFile fileNewTmp(strNewTmpFile);
        if (!fileNewTmp.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(strNewTmpFile),
                                      fileNewTmp.errorString()));
            return false;
        }

        // saving to tmpnew
        {
            QTextStream out(&fileNewTmp);
            out.setCodec("UTF-8");
        #ifndef QT_NO_CURSOR
            QApplication::setOverrideCursor(Qt::WaitCursor);
        #endif
            out << ui->plainTextEdit->toPlainText();
        #ifndef QT_NO_CURSOR
            QApplication::restoreOverrideCursor();
        #endif
        }
    } // file closed

    QString replaceError;
    if(!Move3Files(strFileName, strNewTmpFile, strBackupFile,&replaceError))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot replace file %1 %2 %3:%4.")
                             .arg(
                                 QDir::toNativeSeparators(strFileName),
                                 QDir::toNativeSeparators(strNewTmpFile),
                                 QDir::toNativeSeparators(strBackupFile),
                                 replaceError
                                 )
                             );
    }

    setCurrentFile(strFileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::updateTitle()
{
    QString title=windowFilePath();
    if(ui->plainTextEdit->document()->isModified())
        title+="[*]";
    title += " - ";
    title += qAppName();
    setWindowTitle(title);
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);

    updateTitle();
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->plainTextEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::on_action_New_triggered()
{
    if (maybeSave()) {
        ui->plainTextEdit->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::on_action_Open_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        qAppName(),
                                                        fileDirectory_);
        if (fileName.isEmpty())
            return;

        fileDirectory_ = QFileInfo(fileName).path();
        loadFile(fileName);
    }
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (ui->plainTextEdit->document()->isModified())
            on_action_Save_triggered();
    }
}
#endif
