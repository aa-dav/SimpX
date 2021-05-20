#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( &timer, SIGNAL(timeout()), this, SLOT(on_Timer()));
    timer.start( 0 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Timer()
{
    //close();
    ui->glWidget->update();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
