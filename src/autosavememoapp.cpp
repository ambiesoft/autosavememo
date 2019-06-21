#include <QRect>
#include <QDesktopWidget>
#include <QStandardPaths>
#include <QApplication>

#include "../../lsMisc/stdQt/settings.h"

#include "consts.h"
#include "autosavememoapp.h"

using namespace Consts;
using namespace AmbiesoftQt;

AutosavememoApp* theApp;

AutosavememoApp::AutosavememoApp(int &argc, char **argv):
    QApplication(argc, argv)
{
    theApp = this;
}

AutosavememoApp::~AutosavememoApp()
{}

bool AutosavememoApp::readSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    geometry_ = settings.value(SECTION_GEOMETRY, QByteArray()).toByteArray();
    fileDirectory_ = settings.valueString(SECTION_FILEOPENDIRECTORY);
    if(fileDirectory_.isEmpty())
        fileDirectory_ = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    setWordWrap(settings.valueBool(SECTION_WORDWRAP));

    if(settings.value(SECTION_FONT).isValid())
        fontString_ = settings.valueString(SECTION_FONT);

    return true;
}
bool AutosavememoApp::writeSettings()
{
    IniSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(SECTION_GEOMETRY, geometry_);

    settings.setValue(SECTION_FILEOPENDIRECTORY, fileDirectory_);
    settings.setValue(SECTION_WORDWRAP, wordWrap());

    settings.setValue(SECTION_FONT, fontString_);
    return true;
}
