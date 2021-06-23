#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "simp4completer.h"
#include "simp4highlighter.h"
#include "simpleton4disasm.h"

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

#define INIT_PREDEF_FILE( name ) { \
    QListWidgetItem *item = new QListWidgetItem( name ); \
    item->setData( Qt::ItemDataRole::UserRole, readResourceAsString( ":/asm/" name ) ); \
    ui->filesList->addItem( item ); };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , simp( 64 )
    , asm4( simp.getMMU() )
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusLabel = new QLabel();
    ui->statusBar->addWidget( statusLabel );
    //connect( ui->glWidget, SIGNAL( painted() ), this, SLOT(on_Timer()));

    ui->codeEditor->setCompleter( new Simp4Completer() );
    ui->codeEditor->setHighlighter( new Simp4Highlighter() );
    ui->codeEditor->setWordWrapMode( QTextOption::NoWrap );

    Q_INIT_RESOURCE( main );

    INIT_PREDEF_FILE( "simpx.inc" );
    INIT_PREDEF_FILE( "test0.asm" );
    INIT_PREDEF_FILE( "font-00.asm" );
    INIT_PREDEF_FILE( "simple_lib.inc" );

#if BUILD_WEBASSEMBLY == 1
    asm4.setSourceFileProvider( std::make_shared<FileSetProvider>( ui->filesList ) );
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

    timer.setSingleShot( false );
    connect( &timer, SIGNAL( timeout() ), this, SLOT( on_Timer() ) );
    timer.start( 17 );
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

QListWidgetItem *MainWindow::getFilesSelItem()
{
    QList< QListWidgetItem* > sels = ui->filesList->selectedItems();
    QListWidgetItem *ret = nullptr;
    if ( sels.size() > 0 )
    {
        ret = sels.at( 0 );
    }
    return ret;
}

QListWidgetItem *MainWindow::findFilesItem( const QString &name )
{
    for ( int i = 0; i < ui->filesList->count(); i++ )
    {
        if ( ui->filesList->item( i )->text() == name )
        {
            return ui->filesList->item( i );
        }
    }
    return nullptr;
};


void MainWindow::fileContentReady(const QString &fname, const QByteArray &arr)
{
    QString shortName = QFileInfo( fname ).fileName();
    QString content = QString::fromUtf8( arr );
    QListWidgetItem *item = findFilesItem( shortName );
    if ( item )
    {
        item->setData( Qt::ItemDataRole::UserRole, content );
        if ( item == getFilesSelItem() )
            ui->codeEditor->setPlainText( content );
        else
            ui->filesList->setCurrentItem( item );
    }
    else
    {
        item = new QListWidgetItem( shortName );
        item->setData( Qt::ItemDataRole::UserRole, content );
        ui->filesList->addItem( item );
        ui->filesList->setCurrentItem( item );
    }
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
        if ( simp.getCPU().getFlag( Simpleton::FLAG_DEBUG ) )
        {
            simp.getCPU().setFlag( Simpleton::FLAG_DEBUG, false );
            run = false;
            //ui->debuggerEditor->clear();
            QString dump = QString::fromStdString( simpleDump( simp.getCPU() ) );
            ui->debuggerEditor->moveCursor (QTextCursor::End);
            ui->debuggerEditor->insertPlainText( dump );
            ui->tabWidget->setCurrentWidget( ui->debuggerTab );
        }
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

QListWidgetItem *MainWindow::saveCurrentFile()
{
    QListWidgetItem *item = getFilesSelItem();
    if ( item )
    {
        item->setData( Qt::ItemDataRole::UserRole, ui->codeEditor->toPlainText() ); // save it
    }
    return item;
}

void MainWindow::on_actionSave_triggered()
{
    QListWidgetItem *item = saveCurrentFile();
    if ( item )
        QFileDialog::saveFileContent( item->data( Qt::ItemDataRole::UserRole ).toByteArray(), item->text() );
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

    QListWidgetItem *item = saveCurrentFile();
    if ( item == nullptr )
    {
        ui->msgEdit->setPlainText( "There is no selected file!" );
        return;
    }
    if ( !asm4.parseFile( item->text().toStdString() ) )
    {
        ui->msgEdit->setPlainText( asm4.getErrorMessage().c_str() );
        ui->tabWidget->setCurrentWidget( ui->editorTab );
    }
    else
    {
        run = true;
        ui->msgEdit->setPlainText( "Ok." );
        ui->debuggerEditor->clear();
        ui->tabWidget->setCurrentWidget( ui->videoTab );
    }
}

void MainWindow::on_filesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    // save current one...
    if ( (previous != nullptr) )
    {
        previous->setData( Qt::ItemDataRole::UserRole, ui->codeEditor->toPlainText() );
    }
    if ( current != nullptr )
    {
        ui->codeEditor->setPlainText( current->data( Qt::ItemDataRole::UserRole ).toString() );
        ui->fileNameEdit->setText( current->text() );
        ui->codeEditor->setReadOnly( false );
        ui->fileNameEdit->setReadOnly( false );
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
        delete item;
    }
}

void MainWindow::on_fileNameEdit_textEdited(const QString &arg1)
{
    QListWidgetItem *item = getFilesSelItem();
    if ( item )
    {
        item->setText( arg1 );
    }
    QImage img;
}



void MainWindow::on_actionImageToAsm_triggered()
{
    QFileDialog::getOpenFileContent(
        "All files (*.*) ;; Image files (*.png *.bmp *.jpg *.gif)",
        [&](const QString &fname, const QByteArray &arr)
        {
            QString baseName = QFileInfo( fname ).completeBaseName();
            QImage img;
            if ( !img.loadFromData( arr ) )
            {
                ui->msgEdit->setPlainText( "Cannot read image!" );
                return;
            }
            ui->msgEdit->setPlainText( QString( "image is %0 x %1" ).arg( img.width() ).arg( img.height() ) );
            QString out;
            out += baseName + "\n";
            int acc = 0;
            int quot = 0;
            for ( int j = 0; j < img.height(); j++ )
                for ( int i = 0; i < img.width(); i++ )
                {
                    QRgb pix = img.pixel( i, j );
                    int clr = (qGray( pix ) > 100) ? 15 : 0;
                    acc = ((acc << 4) | clr);
                    quot++;
                    if ( quot == 4 )
                    {
                        QString num = QString::number( acc, 16 );
                        while ( num.size() < 4 )
                            num = "0" + num;
                        out += "\t\tdw $" + num + "\n";
                        acc = 0;
                        quot = 0;
                    }
                }
            out += baseName + "_end\n";
            QFileDialog::saveFileContent( out.toUtf8(), baseName + ".asm" );
        } );

}

void MainWindow::on_actionResume_triggered()
{
    run = true;
    ui->glWidget->update();
    ui->tabWidget->setCurrentWidget( ui->videoTab );
}