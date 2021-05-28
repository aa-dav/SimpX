#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QLabel>
#include <QSettings>
#include "utils.h"
#include "simpleton4.h"
#include "simpleton4asm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel *statusLabel = nullptr;
    FpsMeter fps;
    Simpleton::MMU64 mmu;
    Simpleton::Machine simp;
    Simpleton::Assembler asm4;
    bool run = false;

    void setViewSize( int coef );

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Timer();
    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionView200_triggered();

    void on_actionView300_triggered();

    void on_actionView400_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
