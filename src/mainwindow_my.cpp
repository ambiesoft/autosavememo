#include <QMessageBox>
//#include <QSettings>
#include <QTextStream>
#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;

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


