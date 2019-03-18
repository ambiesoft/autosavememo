#include <QMessageBox>
//#include <QSettings>
#include <QTextStream>
#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>
#include <QDebug>
#include <QTextCodec>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "helper.h"

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

bool MainWindow::getByteArrayFromFile(QFile& file,
                                      QByteArray& qba,
                                      const qint64& maxsize)
{
    qint64 fileSize = file.size();
    if(maxsize != -1 && fileSize > maxsize)
    {
        QMessageBox::warning(this,
                             qAppName(),
                             tr("File size is too large."));
        return false;
    }

    qba = file.peek(fileSize);
    return true;
}
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) // | QFile::Text))
    {
        QMessageBox::warning(this,
                             qAppName(),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QByteArray allBytes ;
    if(!getByteArrayFromFile(file, allBytes, 10 * 1024 * 1024))
        return;

    const bool hasByteOrderMark = QTextCodec::codecForUtfText(allBytes, nullptr) != nullptr;
    QTextStream in(&file);
    if(hasByteOrderMark)
        in.setAutoDetectUnicode(true);
    else
    {
        QTextCodec* codec = nullptr;
        aaa(allBytes, codec);
        in.setCodec("UTF-8");
    }

    {
        CWaitCursor wc;
        ui->plainTextEdit->setPlainText(in.readAll());
    }

    qDebug() << "codec = " << in.codec()->name();
    setCurrentFile(fileName, allBytes, in.codec(), hasByteOrderMark);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &strFileName)
{
    QString strNewTmpFile = (strFileName+".saving");
    QString strBackupFile;//= (strFileName+".bkmemo");

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
            out.setGenerateByteOrderMark(curHasBOM_);
            if(curCodec_)
                out.setCodec(curCodec_);
            else
                out.setCodec("UTF-8");


            {
                CWaitCursor wc;
                out << ui->plainTextEdit->toPlainText();
            }

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

    // open to get bytearray
    QFile file(strFileName);
    if (!file.open(QFile::ReadOnly)) // | QFile::Text))
    {
        QMessageBox::warning(this,
                             qAppName(),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(strFileName), file.errorString()));
        return true;
    }
    QByteArray qba;
    getByteArrayFromFile(file, qba, -1);

    setCurrentFile(strFileName,qba, curCodec_);
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
void MainWindow::setCurrentFile(const QString &fileName,
                                const QByteArray& allBytes,
                                QTextCodec* codec,
                                bool hasBOM)
{
    curFile_ = fileName;
    curAllBytes_ = allBytes;
    curCodec_=codec;
    curHasBOM_ = hasBOM;

    ui->action_BOM->setChecked(curHasBOM_);

    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile_;
    if (curFile_.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);

    updateTitle();
}


