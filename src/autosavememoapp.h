#ifndef AUTOSAVEMEMOAPP_H
#define AUTOSAVEMEMOAPP_H

#include <QApplication>
#include <QByteArray>

class AutosavememoApp : public QApplication
{
    bool wordWrap_;
    QByteArray geometry_;
    QString fileDirectory_;
public:
    AutosavememoApp(int &argc, char **argv);
    virtual ~AutosavememoApp();

    bool readSettings();
    bool writeSettings();

    bool wordWrap() const {
        return wordWrap_;
    }
    void setWordWrap(bool b) {
        wordWrap_ = b;
    }

    QByteArray geometry() const {
        return geometry_;
    }
    void setGeometry(const QByteArray& g) {
        geometry_ = g;
    }

    QString fileDirectory() const {
        return fileDirectory_;
    }
    void setFileDirectory(const QString& s) {
        fileDirectory_ = s;
    }

};

extern AutosavememoApp* theApp;

#endif // AUTOSAVEMEMOAPP_H
