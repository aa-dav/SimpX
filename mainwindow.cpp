#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

static QString orgName = "AlxSoft";
static QString domName = "alxhost.tk";
static QString appName = "SimpX";

#define INIT_SETTINGS( name ) QSettings name( QSettings::IniFormat, QSettings::UserScope, orgName, appName )

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    ui->glWidget->update();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
