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

void MainWindow::setViewSize(int coef)
{
    ui->glWidget->setFixedSize( 256 * coef, 192 * coef );
    ui->centralwidget->adjustSize();
    ui->glWidget->setMinimumSize( 0, 0 );
    ui->glWidget->setMaximumSize( 16777215, 16777215 );
}

inline uint16_t makeColor( int r, int g, int b )
{
    return ((r & 0x1F) << 10) | ((g & 0x1F) << 5) | ((b & 0x1F) << 0);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , simp( 64 )
    , asm4( simp.getMMU() )
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusLabel = new QLabel();
    ui->statusBar->addWidget( statusLabel );
    connect( ui->glWidget, SIGNAL( painted() ), this, SLOT(on_Timer()));

    INIT_SETTINGS( sett );
    sett.beginGroup( "General" );
    lastOpenDir = sett.value( "lastOpenDir", "" ).toString();
    sett.endGroup();
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
    sett.beginGroup( "General" );
    sett.setValue( "lastOpenDir",   lastOpenDir );
    sett.endGroup();
    sett.beginGroup( "MainWindow" );
    sett.setValue( "geom",          saveGeometry() );
    sett.setValue( "state",         saveState() );
    sett.endGroup();
    sett.sync();
    delete ui;
}

void MainWindow::on_Timer()
{
    fps.tick();
    statusLabel->setText( QStringLiteral( "fps: %1, run: %2, clocks: %3" ).arg( fps.getFps() ).arg( run ).arg( simp.getClocks() ) );
    if ( run )
    {
        simp.stepFrame();
    }
    ui->glWidget->setBitmap( simp.getFramePtr() );
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
              this, "Open asm file", lastOpenDir,
              "All files (*.*) ;; Assembler files (*.asm *.inc)" );

    if( !filename.isNull() )
    {
        lastOpenDir = QFileInfo( filename ).absolutePath();
        simp.reset();
        asm4.reset();
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

void MainWindow::on_actionView200_triggered()
{
    setViewSize( 2 );
}

void MainWindow::on_actionView300_triggered()
{
    setViewSize( 3 );
}

void MainWindow::on_actionView400_triggered()
{
    setViewSize( 4 );
}
