#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSessionManager;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    using ParentClass = QMainWindow;

    QString fileDirectory_;

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

private:
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName, QTextCodec* codec=nullptr);
    void updateTitle();

    Ui::MainWindow *ui;

    QString curFile_;
    QTextCodec* curCodec_=nullptr;
};

#endif // MAINWINDOW_H
