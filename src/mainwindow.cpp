#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
#include <QFileDialog>
#include <QSessionManager>
//#include <QStandardPaths>
#include <QTimer>
#include <QDebug>
#include <QTextCodec>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "autosavememoapp.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;


void MainWindow::onDeactivated()
{
    if(savedOnce())
    {
        if(!HasModalWindow())
        {
            if(ui->plainTextEdit->document()->isModified())
            {
                if(!curFile_.isEmpty())
                {
                    on_action_Save_triggered();
                }
            }
        }
    }
}

// save when deactivated
bool MainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
        // save document after 500ms
        QTimer::singleShot(500, this, &MainWindow::onDeactivated);
    }

    return ParentClass::event(e);;
}




void MainWindow::documentWasModified()
{
    updateTitle();
    setWindowModified(ui->plainTextEdit->document()->isModified());
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        // writeSettings();
        theApp->setGeometry(saveGeometry());
        event->accept();
    } else {
        event->ignore();
    }
}



bool MainWindow::on_action_SaveAs_triggered()
{
    QFileDialog dialog(this);
    dialog.setDirectory(theApp->fileDirectory());
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    QString file = dialog.selectedFiles().first();
    theApp->setFileDirectory(QFileInfo(file).path());
    return saveFile(file, curCodec() ? curCodec() : GetUtf8Codec());
}

bool MainWindow::on_action_Save_triggered()
{
    if (curFile_.isEmpty()) {
        // return on_action_SaveAs_triggered();
        QString newFile;
        QString baseDir=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        if(baseDir.isEmpty() || !QDir(baseDir).exists())
        {
            Alert(this, tr("Failed to get the desktop directory."));
            return false;
        }
        for(int i=0 ; i < 32; ++i)
        {
            newFile = pathCombine(baseDir,
                                  QString("autosavememo-%1.txt").arg(QTime::currentTime().msecsSinceStartOfDay()));
            if(!QFile::exists(newFile) && !QDir(newFile).exists())
                break;
        }
        if(newFile.isEmpty())
        {
            Alert(this, tr("Failed to create new file in '%1'").arg(baseDir));
            return false;
        }

        curFile_=newFile;
    }
    return saveFile(curFile_);

}




void MainWindow::on_action_New_triggered()
{
    if (maybeSave()) {
        ui->plainTextEdit->clear();
        setCurrentFile(QString(), QByteArray());
    }
}

void MainWindow::on_action_Open_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        qAppName(),
                                                        theApp->fileDirectory());
        if (fileName.isEmpty())
            return;

        theApp->setFileDirectory(QFileInfo(fileName).path());
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

void MainWindow::on_action_Wrap_toggled(bool b)
{
    ui->plainTextEdit->setWordWrapMode(b ? QTextOption::WordWrap : QTextOption::NoWrap);
    theApp->setWordWrap(b);
}



void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this,
                             qAppName(),
                             qAppName() + " v" + QApplication::applicationVersion());
}




void MainWindow::uncheckAllCharcterCoding()
{

}

void MainWindow::onCharacterCode_AboutToShow()
{
#define UNCHECKIFCHEKED(action) do { if(action->isChecked()) { action->setChecked(false); } } while(false)
    UNCHECKIFCHEKED(ui->action_UTF8);
    UNCHECKIFCHEKED(ui->action_UTF8_with_BOM);
    UNCHECKIFCHEKED(ui->action_EUCJP);
    UNCHECKIFCHEKED(ui->action_ShiftJIS);
#undef UNCHECKIFCHEKED

    if(curCodec()==QTextCodec::codecForName("UTF-8"))
    {
        if(curHasBOM())
            ui->action_UTF8_with_BOM->setChecked(true);
        else
            ui->action_UTF8->setChecked(true);
    }
    else if(curCodec()==QTextCodec::codecForName("EUC-JP"))
    {
        ui->action_EUCJP->setChecked(true);
    }
    else if(curCodec()==QTextCodec::codecForName("ShiftJIS"))
    {
        ui->action_ShiftJIS->setChecked(true);
    }
    else if(curCodec()==QTextCodec::codecForName("UTF-16LE"))
    {
        if(curHasBOM())
            ui->action_UTF_16LE_with_BOM->setChecked(true);
        else
            ui->action_UTF_16LE->setChecked(true);
    }
    else if(curCodec()==QTextCodec::codecForName("UTF-16BE"))
    {
        if(curHasBOM())
            ui->action_UTF_16BE_with_BOM->setChecked(true);
        else
            ui->action_UTF_16BE->setChecked(true);
    }
    else
    {
        Q_ASSERT(false);
        qWarning() << "Unknown Codec";
    }
}

// qDebug() << QTextCodec::availableCodecs();
//("UTF-8", "ISO-8859-1", "latin1", "CP819", "IBM819", "iso-ir-100",
//"csISOLatin1", "ISO-8859-15", "latin9", "UTF-32LE", "UTF-32BE", "UTF-32",
//"UTF-16LE", "UTF-16BE", "UTF-16", "System", "Big5-HKSCS", "Big5", "Big5-ETen",
//"CP950", "windows-949", "CP949", "EUC-KR", "Shift_JIS", "SJIS", "MS_Kanji",
//"ISO-2022-JP", "JIS7", "EUC-JP", "GB2312", "GBK", "CP936", "MS936", "windows-936",
//"GB18030", "hp-roman8", "roman8", "csHPRoman8", "TIS-620", "ISO 8859-11",
//"WINSAMI2", "WS2", "macintosh", "Apple Roman", "MacRoman", "windows-1258",
//"CP1258", "windows-1257", "CP1257", "windows-1256", "CP1256", "windows-1255",
//"CP1255", "windows-1254", "CP1254", "windows-1253", "CP1253", "windows-1252",
//"CP1252", "windows-1251", "CP1251", "windows-1250", "CP1250", "IBM866", "CP866",
//"csIBM866", "IBM874", "CP874", "IBM850", "CP850", "csPC850Multilingual", "ISO-8859-16",
//"iso-ir-226", "latin10", "ISO-8859-14", "iso-ir-199", "latin8", "iso-celtic",
//"ISO-8859-13", "ISO-8859-10", "iso-ir-157", "latin6", "ISO-8859-10:1992", "csISOLatin6",
//"ISO-8859-9", "iso-ir-148", "latin5", "csISOLatin5", "ISO-8859-8", "ISO 8859-8-I",
//"iso-ir-138", "hebrew", "csISOLatinHebrew", "ISO-8859-7", "ECMA-118", "greek",
//"iso-ir-126", "csISOLatinGreek", "ISO-8859-6", "ISO-8859-6-I", "ECMA-114", "ASMO-708",
//"arabic", "iso-ir-127", "csISOLatinArabic", "ISO-8859-5", "cyrillic", "iso-ir-144",
//"csISOLatinCyrillic", "ISO-8859-4", "latin4", "iso-ir-110", "csISOLatin4", "ISO-8859-3",
//"latin3", "iso-ir-109", "csISOLatin3", "ISO-8859-2", "latin2", "iso-ir-101", "csISOLatin2",
//"KOI8-U", "KOI8-RU", "KOI8-R", "csKOI8R", "iscii-mlm", "iscii-knd", "iscii-tlg",
//"iscii-tml", "iscii-ori", "iscii-gjr", "iscii-pnj", "iscii-bng", "iscii-dev", "TSCII")

void MainWindow::on_action_UTF8_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-8");
    setCurHasBOM(false);
}

void MainWindow::on_action_UTF8_with_BOM_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-8");
    setCurHasBOM(true);
}

void MainWindow::on_action_EUCJP_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("EUC-JP");
    setCurHasBOM(false);
}

void MainWindow::on_action_ShiftJIS_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("Shift_JIS");
    setCurHasBOM(false);
}

void MainWindow::on_action_UTF_16LE_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-16LE");
    setCurHasBOM(true);
}

void MainWindow::on_action_UTF_16LE_with_BOM_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-16LE");
    setCurHasBOM(false);
}



void MainWindow::on_action_UTF_16BE_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-16BE");
    setCurHasBOM(false);
}

void MainWindow::on_action_UTF_16BE_with_BOM_toggled(bool b)
{
    if(!b)
        return;

    setCurCodec("UTF-16BE");
    setCurHasBOM(true);
}

void MainWindow::on_action_Cut_triggered()
{
    ui->plainTextEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->plainTextEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_action_Undo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    ui->plainTextEdit->redo();
}

void MainWindow::on_action_Zoom_In_triggered()
{
    ui->plainTextEdit->zoomIn();
}

void MainWindow::on_actio_Zoom_out_triggered()
{
    ui->plainTextEdit->zoomOut();
}
