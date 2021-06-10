#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "simp4completer.h"
#include "simp4highlighter.h"

#if BUILD_WEBASSEMBLY != 1
static const char* orgName = "AlxSoft";
static const char* domName = "alxhost.tk";
static const char* appName = "SimpX";
#endif

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

QString readResourceAsString( QString name )
{
    QFile file( name );
    file.open( QIODevice::ReadOnly );
    QTextStream strm( &file );
    strm.setCodec( "UTF-8" );
    return strm.readAll();
}

#define INIT_PREDEF_FILE( name ) \
    files->addContent( name, readResourceAsString( ":/asm/" name ) ); \
    ui->filesList->addItem( name );

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

    files = std::make_shared<FileSetProvider>();

    ui->codeEditor->setCompleter( new Simp4Completer() );
    ui->codeEditor->setHighlighter( new Simp4Highlighter() );
    ui->codeEditor->setWordWrapMode( QTextOption::NoWrap );

    Q_INIT_RESOURCE( main );

    INIT_PREDEF_FILE( "simpx.inc" );
    INIT_PREDEF_FILE( "test0.asm" );

#if BUILD_WEBASSEMBLY == 1
    asm4.setSourceFileProvider( files );
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
#if BUILD_WEBASSEMBLY == 1
#else
    INIT_SETTINGS( sett );
    sett.beginGroup( "General" );
    sett.setValue( "lastOpenDir",   lastOpenDir );
    sett.endGroup();
    sett.beginGroup( "MainWindow" );
    sett.setValue( "geom",          saveGeometry() );
    sett.setValue( "state",         saveState() );
    sett.endGroup();
    sett.sync();
#endif
    delete ui;
}

int MainWindow::getFilesSelRow()
{
    QListWidgetItem* item = getFilesSelItem();
    if ( item != nullptr )
    {
        return ui->filesList->row( item );
    }
    else
        return -1;
}

QListWidgetItem *MainWindow::getFilesSelItem()
{
    QList< QListWidgetItem* > sels = ui->filesList->selectedItems();
    if ( sels.size() > 0 )
        return sels.at( 0 );
    else
        return nullptr;
}

void MainWindow::fileContentReady(const QString &fname, const QByteArray &arr)
{
    QString shortName = QFileInfo( fname ).fileName();
    QString content = QString::fromUtf8( arr );
    int pos = files->addContent( shortName, content );
    int selRow = getFilesSelRow();
    if ( pos <= selRow )
    {
        if ( pos == selRow )
            ui->codeEditor->setPlainText( content );
        else
            ui->filesList->setCurrentRow( pos );
    }
    else
    {
        ui->filesList->addItem( shortName );
        ui->filesList->setCurrentRow( pos );
    };
}

void MainWindow::on_Timer()
{
    fps.tick();
    statusLabel->setText( QStringLiteral( "fps: %1, clocks: %2" ).arg( fps.getFps() ).arg( simp.getClocks() ) );
    if ( run )
    {
#if PPU_SOFT_RENDER == 1
        simp.stepFrame( nullptr, (uint32_t *) ui->glWidget->getImage().bits() );
#else
        simp.stepFrame( (uint16_t *) ui->glWidget->getImage().bits(), nullptr );
#endif
        ui->glWidget->update();
    }
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
                [&](const QString &fname, const QByteArray &arr) { fileContentReady( fname, arr ); } );
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
        fileContentReady( filename, strm.readAll().toUtf8() );
    }
#endif
}

void MainWindow::saveCurrentFile()
{
    int curRow = getFilesSelRow();
    if ( curRow >= 0 )
    {
        files->setContent( curRow, ui->codeEditor->toPlainText() ); // save it
    }
}

void MainWindow::on_actionSave_triggered()
{
    saveCurrentFile();
    int row = getFilesSelRow();
    if ( row >= 0 )
        QFileDialog::saveFileContent( files->getContent( row ).toUtf8(), files->getName( row ) );
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

    saveCurrentFile();
    int curRow = getFilesSelRow();
    if ( curRow < 0 )
    {
        ui->msgEdit->setPlainText( "There is no selected file!" );
        return;
    }
    if ( !asm4.parseFile( files->getName( curRow ).toStdString() ) )
    {
        ui->msgEdit->setPlainText( asm4.getErrorMessage().c_str() );
        ui->tabWidget->setCurrentIndex( 1 );
    }
    else
    {
        run = true;
        ui->msgEdit->setPlainText( "Ok." );
        ui->tabWidget->setCurrentIndex( 0 );
    }
}

void MainWindow::on_filesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    int row;
    // save current one...
    if ( (previous != nullptr) )
    {
        row = ui->filesList->row( previous );
        if ( (row >= 0) )
            files->setContent( row, ui->codeEditor->toPlainText() );
    }
    if ( current != nullptr )
    {
        row = ui->filesList->row( current );
        if ( row >= 0 )
        {
            ui->codeEditor->setPlainText( files->getContent( row ) );
            ui->fileNameEdit->setText( files->getName( row ) );
            ui->codeEditor->setReadOnly( false );
            ui->fileNameEdit->setReadOnly( false );
        }
    }
    else
    {
        ui->codeEditor->setPlainText( "" );
        ui->codeEditor->setReadOnly( true );
        ui->fileNameEdit->setText( "" );
        ui->fileNameEdit->setReadOnly( true );
    }
}

void MainWindow::on_addFileBtn_clicked()
{
    fileContentReady( "New file", "" );
}

void MainWindow::on_delFileBtn_clicked()
{
    QListWidgetItem *item = getFilesSelItem();
    if ( item )
    {
        int row = ui->filesList->row( item );
        delete item;
        files->remove( row );
    }
}

void MainWindow::on_fileNameEdit_textEdited(const QString &arg1)
{
    QListWidgetItem *item = getFilesSelItem();
    if ( item )
    {
        item->setText( arg1 );
        files->setName( ui->filesList->row( item ), arg1 );
    }
}
