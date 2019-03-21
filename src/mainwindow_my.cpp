#include <QMessageBox>
//#include <QSettings>
#include <QTextStream>
#include <QDir>
#include <QFile>
//#include <QDesktopWidget>
//#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
//#include <QStandardPaths>
#include <QDebug>
#include <QTextCodec>
#include <QIODevice>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "helper.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;

namespace {
    QIODevice::OpenModeFlag bbb()
    {
        // if 0 returns, text is saved LF
        // if TEXT return text is saved CRLF
        return QFile::Text;
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
    if (Q_UNLIKELY(!file.open(QFile::ReadOnly| bbb())))
    {
        Alert(this,
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
        // First try to dectect by Google,
        // if it failed or not UTF,
        // Use ICU
        QTextCodec* codec = nullptr;
        if (!GetDetectedCodecGoogle(allBytes, codec) || !codec ||
                (codec->name().left(3) != "UTF"))
        {
            codec = nullptr;
            if (Q_UNLIKELY(!GetDetectedCodecICU(allBytes, codec) || !codec))
            {
                Alert(this,tr("Could not detect codec."));
                return;
            }
        }
        in.setAutoDetectUnicode(false);
		in.setCodec(codec);
    }

    {
        CWaitCursor wc;
        ui->plainTextEdit->setPlainText(in.readAll());
    }

    qDebug() << "codec = " << in.codec()->name();
    setCurrentFile(fileName, allBytes, in.codec(), hasByteOrderMark);
    setSavedOnce(false);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &strFileName,
                          QTextCodec* codec)
{
    if(!codec)
        codec = curCodec();

    if(Q_UNLIKELY(!codec))
    {
        Alert(this,tr("Codec not set."));
        return false;
    }

    QString strNewTmpFile = (strFileName+".saving");
    QString strBackupFile;//= (strFileName+".bkmemo");


    {
        QFile fileNewTmp(strNewTmpFile);
        if (Q_UNLIKELY(!fileNewTmp.open(QFile::WriteOnly | bbb() )))
        {
            Alert(this,
                  tr("Cannot write file %1:\n%2.")
                  .arg(QDir::toNativeSeparators(strNewTmpFile), fileNewTmp.errorString()));
            return false;
        }



        // saving to tmpnew
        {
            QTextStream out(&fileNewTmp);
            out.setGenerateByteOrderMark(curHasBOM());
            out.setCodec(codec);

            {
                CWaitCursor wc;
                out << ui->plainTextEdit->toPlainText();
            }

        }
    } // file closed

    QString replaceError;
    if(Q_UNLIKELY(!Move3Files(strFileName, strNewTmpFile, strBackupFile,&replaceError)))
    {
        Alert(this,
              tr("Cannot replace file %1 %2:%4.")
                             .arg(
                                 QDir::toNativeSeparators(strFileName),
                                 QDir::toNativeSeparators(strNewTmpFile),
                                 // QDir::toNativeSeparators(strBackupFile),
                                 replaceError
                                 )
                             );
        return false;
    }

    // open to get bytearray
    QFile file(strFileName);
    if (Q_UNLIKELY(!file.open(QFile::ReadOnly | bbb() )))
    {
        Alert(this,
              tr("Cannot read file %1:\n%2.")
              .arg(QDir::toNativeSeparators(strFileName), file.errorString()));
        return true;
    }
    QByteArray qba;
    getByteArrayFromFile(file, qba, -1);

    setCurrentFile(strFileName,qba, codec, curHasBOM());

    setSavedOnce(true);

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
void MainWindow::updateStatusText()
{
    QString statusText;
    if(curCodec())
        statusText += curCodec()->name();
    if(curHasBOM())
        statusText += " (BOM)";

    statusLabelCodec_.setText(statusText);
}
void MainWindow::setCurrentFile(const QString &fileName,
                                const QByteArray& allBytes,
                                QTextCodec* codec,
                                bool hasBOM)
{
    curFile_ = fileName;
    curAllBytes_ = allBytes;
    setCurCodec(codec);
    setCurHasBOM(hasBOM);



    // ui->action_BOM->setChecked(curHasBOM());

    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile_;
    if (curFile_.isEmpty())
    {
        setSavedOnce(false);
        shownName = "untitled.txt";
    }
    setWindowFilePath(shownName);

    // update statusbar






    updateTitle();
    updateStatusText();
}


void MainWindow::setCurCodec(const char* codecText)
{
    setCurCodec(QTextCodec::codecForName(codecText));
}
void MainWindow::setCurCodec(QTextCodec* codec)
{
    if(!codec)
        codec = GetUtf8Codec();

    curCodec_x = codec;
    Q_ASSERT(curCodec_x);
    updateStatusText();
}
