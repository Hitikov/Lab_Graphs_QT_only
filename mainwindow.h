#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_userAddNode_clicked();

public slots:

    void AddNewPathList(QString argv1);
    void PathListClear();
    void BlockUI();
    void UnBlockUI();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
