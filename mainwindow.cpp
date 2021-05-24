#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

static QString orgName = "AlxSoft";
static QString domName = "alxhost.tk";
static QString appName = "SimpX";

#define INIT_SETTINGS( name ) QSettings name( QSettings::IniFormat, QSettings::UserScope, orgName, appName )

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , asm4( &simp4 )
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusLabel = new QLabel();
    ui->statusBar->addWidget( statusLabel );
    connect( ui->glWidget, SIGNAL( painted() ), this, SLOT(on_Timer()));

    INIT_SETTINGS( sett );
    sett.beginGroup( "MainWindow" );
    QVariant val = sett.value( "geom" );
    if ( val.isValid() )
        restoreGeometry( val.toByteArray() );
    val = sett.value( "state" );
    if ( val.isValid() )
        restoreState( val.toByteArray() );
    sett.endGroup();
}

MainWindow::~MainWindow()
{
    INIT_SETTINGS( sett );
    sett.beginGroup( "MainWindow" );
    sett.setValue( "geom",      saveGeometry() );
    sett.setValue( "state",     saveState() );
    sett.endGroup();
    sett.sync();
    delete ui;
}

void MainWindow::on_Timer()
{
    fps.tick();
    statusLabel->setText( QStringLiteral( "fps: %1" ).arg( fps.getFps() ) );
    if ( run )
    {
        for ( int i =0; i < 1000; i++ )
            simp4.step();
    }
    ui->glWidget->setScreen( simp4.getMem() + 32768 );
    ui->glWidget->update();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    run = false;
    QString filename =  QFileDialog::getOpenFileName(
              this, "Open asm file", QDir::currentPath(),
              "All files (*.*) ;; Assembler files (*.asm *.inc)" );

    if( !filename.isNull() )
    {
        if ( !asm4.parseFile( filename.toStdString() ) )
        {
            QMessageBox msg;
            msg.setText( asm4.getErrorMessage().c_str() );
            msg.exec();
        }
        else
            run = true;
    }
}
