#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

class FileFromEditor: public Simpleton::File
{
    int line = 0;
    QStringList strs;
public:
    FileFromEditor( const QStringList &list ): strs( list ) {};

    // File interface
public:
    std::string get_line() override
    {
        if ( line < strs.size() )
            return strs[ line++ ].toStdString();
        else
            return std::string();
    };
    bool eof() override
    {
        return line >= strs.size();
    };
};

class FileProviderFromEditor: public Simpleton::FileProvider
{
    QPlainTextEdit *source = nullptr;
public:
    FileProviderFromEditor( QPlainTextEdit *src ): source( src ) {};

    // FileProvider interface
public:
    std::shared_ptr<Simpleton::File> open(const std::string &name) override
    {
        QString s = source->toPlainText();
        return std::make_shared<FileFromEditor>( s.split( '\n' ) );
    };
};

static const char* orgName = "AlxSoft";
static const char* domName = "alxhost.tk";
static const char* appName = "SimpX";

#define INIT_SETTINGS( name ) QSettings name( QSettings::IniFormat, QSettings::UserScope, orgName, appName )

void MainWindow::setViewSize(int coef)
{
    ui->glWidget->setFixedSize( 256 * coef, 192 * coef );
    ui->centralwidget->resize( 100, 100 );
    ui->centralwidget->adjustSize();
    //resize( 100, 100 );
    adjustSize();
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

#if BUILD_WEBASSEMBLY == 1
    asm4.setSourceFileProvider( std::make_shared<FileProviderFromEditor>( ui->fileEdit ) );
#else
    asm4.setSourceFileProvider( std::make_shared<Simpleton::FileProviderStd>() );

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
#endif
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
#if BUILD_WEBASSEMBLY == 1
    QFileDialog::getOpenFileContent(
        "All files (*.*) ;; Assembler files (*.asm *.inc)",
        [this](const QString &fname, const QByteArray &arr)
        {
            this->ui->fileEdit->setPlainText( QString::fromUtf8( arr ) );
        } );
#else
    QString filename =  QFileDialog::getOpenFileName(
              this, "Open asm file", lastOpenDir,
              "All files (*.*) ;; Assembler files (*.asm *.inc)" );

    if( !filename.isNull() )
    {
        lastOpenDir = QFileInfo( filename ).absolutePath();
        lastOpenFile = QFileInfo( filename ).absoluteFilePath();

        QFile file( lastOpenFile );
        file.open( QIODevice::ReadOnly );
        QTextStream strm( &file );
        strm.setCodec( "UTF-8" );
        ui->fileEdit->setPlainText( strm.readAll() );
    }
#endif
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

void MainWindow::on_actionStop_triggered()
{
    run = false;
}

void MainWindow::on_actionRun_triggered()
{
    run = false;
    simp.reset();
    asm4.reset();
    if ( !asm4.parseFile( lastOpenFile.toStdString() ) )
    {
        ui->msgEdit->setText( QString::fromStdString( asm4.getErrorMessage() ) );
        ui->tabWidget->setCurrentIndex( 1 );
    }
    else
    {
        run = true;
        ui->msgEdit->setText( "Ok." );
        ui->tabWidget->setCurrentIndex( 0 );
    }
}
