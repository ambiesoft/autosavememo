#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
// #include <QTextCodec>

QT_BEGIN_NAMESPACE
class QSessionManager;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class QFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    using ParentClass = QMainWindow;

    QString fileDirectory_;
    bool getByteArrayFromFile(QFile& file,
                                    QByteArray& qba,
                                    const qint64& maxsize);
    QLabel statusAutomode_x;
    QLabel statusLabelCodec_;

    bool savedOnce_xxx = false;
    bool savedOnce() const {
        return savedOnce_xxx;
    }
    void setSavedOnce(bool b) {
        savedOnce_xxx = b;
        statusAutomode_x.setText(tr("Autosave:") + " " + (b ? "ON" : "OFF"));
    }
    void uncheckAllCharcterCoding();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void loadFile(const QString &fileName);
    void onDeactivated();

private:
    bool maybeSave();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void documentWasModified();
    bool on_action_Save_triggered();
    void on_action_New_triggered();
    void on_action_Open_triggered();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

    void on_action_Wrap_toggled(bool b);
    bool on_action_SaveAs_triggered();
    void on_action_About_triggered();
    void on_action_UTF8_without_BOM_toggled(bool b);
    void on_action_UTF8_with_BOM_toggled(bool b);
    void on_action_EUCJP_toggled(bool b);
    void on_action_ShiftJIS_toggled(bool b);
    void onCharacterCode_AboutToShow();
    void on_action_UTF_16LE_toggled(bool arg1);

    void on_action_UTF_16LE_with_BOM_toggled(bool arg1);

private:
    bool saveFile(const QString &fileName,
                  QTextCodec* codec = nullptr);
    void setCurrentFile(const QString &fileName,
                        const QByteArray& allBytes,
                        QTextCodec* codec=nullptr,
                        bool hasBOM=false);
    void updateTitle();
    void updateStatusText();

    Ui::MainWindow *ui;

    // Current file info
    QString curFile_;
    QByteArray curAllBytes_;
    QTextCodec* curCodec_x=nullptr;
    QTextCodec* curCodec() const {
        return curCodec_x;
    }
    void setCurCodec(const char* codecText);
    void setCurCodec(QTextCodec* codec);


    bool curHasBOM_x=false;
    bool curHasBOM() const {
        return curHasBOM_x;
    }
    void setCurHasBOM(bool b) {
        curHasBOM_x=b;
        updateStatusText();
    }
};

#endif // MAINWINDOW_H
