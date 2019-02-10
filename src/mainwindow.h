#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool maybeSave();
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_plainTextEdit_textChanged();

    bool on_action_Save_triggered();



    bool on_actionSave_As_triggered();

private:
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    Ui::MainWindow *ui;

    QString curFile;
};

#endif // MAINWINDOW_H
