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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void loadFile(const QString &fileName);

private:
    bool maybeSave();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_plainTextEdit_textChanged();
    void documentWasModified();
    bool on_action_Save_triggered();
    bool on_actionSave_As_triggered();
    void on_action_New_triggered();
    void on_action_Open_triggered();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    Ui::MainWindow *ui;

    QString curFile;
};

#endif // MAINWINDOW_H
