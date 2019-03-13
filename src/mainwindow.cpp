#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
//#include <QDesktopWidget>
//#include <QApplication>
#include <QFileDialog>
#include <QSessionManager>
//#include <QStandardPaths>

#include "../../lsMisc/stdQt/settings.h"
#include "../../lsMisc/stdQt/stdQt.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;

// save when deactivated
bool MainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::WindowDeactivate)
    {
        if(ui->plainTextEdit->document()->isModified())
        {
            if(!curFile_.isEmpty())
            {
                on_action_Save_triggered();
            }
        }
    }
    return ParentClass::event(e);
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





bool MainWindow::on_action_Save_triggered()
{
    if (curFile_.isEmpty()) {
        return on_action_SaveAs_triggered();
    } else {
        return saveFile(curFile_);
    }
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

void MainWindow::on_action_Wrap_toggled(bool b)
{
    ui->plainTextEdit->setWordWrapMode(b ? QTextOption::WrapAnywhere : QTextOption::NoWrap);
}

bool MainWindow::on_action_SaveAs_triggered()
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

void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this,
                             qAppName(),
                             qAppName() + " v" + QApplication::applicationVersion());
}
