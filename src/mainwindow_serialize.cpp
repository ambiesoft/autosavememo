//#include <QMessageBox>
//#include <QSettings>
//#include <QTextStream>
//#include <QDir>
#include <QDesktopWidget>
#include <QApplication>
//#include <QFileDialog>
//#include <QSessionManager>
#include <QStandardPaths>

#include "../../lsMisc/stdQt/settings.h"
//#include "../../lsMisc/stdQt/stdQt.h"

#include "consts.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;
using namespace Consts;

void MainWindow::readSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value(SECTION_GEOMETRY, QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    fileDirectory_ = settings.valueString(SECTION_FILEOPENDIRECTORY);
    if(fileDirectory_.isEmpty())
        fileDirectory_ = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    bool bWrap = settings.valueBool(SECTION_WORDWRAP);
    ui->action_Wrap->setChecked(bWrap);
    on_action_Wrap_toggled(bWrap);
}
void MainWindow::writeSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(SECTION_GEOMETRY, saveGeometry());

    settings.setValue(SECTION_FILEOPENDIRECTORY, fileDirectory_);
    settings.setValue(SECTION_WORDWRAP, ui->action_Wrap->isChecked());
}

