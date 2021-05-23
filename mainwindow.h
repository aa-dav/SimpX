#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QLabel>
#include <QSettings>
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel *statusLabel = nullptr;
    FpsMeter fps;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Timer();
    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
