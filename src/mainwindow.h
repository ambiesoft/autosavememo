#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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

    void on_action_Wrap_toggled(bool arg1);

    bool on_action_SaveAs_triggered();

    void on_action_About_triggered();



    void on_action_BOM_toggled(bool arg1);

private:
    bool saveFile(const QString &fileName,
                  QTextCodec* codec = nullptr);
    void setCurrentFile(const QString &fileName,
                        const QByteArray& allBytes,
                        QTextCodec* codec=nullptr,
                        bool hasBOM=false);
    void updateTitle();

    Ui::MainWindow *ui;

    QString curFile_;
    QByteArray curAllBytes_;
    QTextCodec* curCodec_=nullptr;
    bool curHasBOM_=false;
};

#endif // MAINWINDOW_H
