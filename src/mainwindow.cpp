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

#if SIMPX_NO_INI == 0
static const char* orgName = "AlxSoft";
//static const char* domName = "alxhost.tk";
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

    connect( ui->glWidget, SIGNAL( keyInput(bool, int, int) ), this, SLOT( on_keyInput(bool, int, int) ) );
    ui->glWidget->installEventFilter( this );
    connect( QGamepadManager::instance(), SIGNAL( connectedGamepadsChanged() ), this, SLOT( on_gamepadsChanged() ) );

    on_gamepadsChanged();

    //ui->codeEditor->setCompleter( new Simp4Completer() );
    ui->codeEditor->setHighlighter( new Simp4Highlighter() );
    ui->codeEditor->setWordWrapMode( QTextOption::NoWrap );
    ui->codeEditor->setTabReplace( false );
    ui->codeEditor->setAutoIndentation( true );
    ui->codeEditor->setAutoParentheses( false );

    Q_INIT_RESOURCE( main );

    INIT_PREDEF_FILE( "test01.asm" );
    INIT_PREDEF_FILE( "test02.asm" );
    INIT_PREDEF_FILE( "test03.asm" );
    INIT_PREDEF_FILE( "test04.asm" );
    INIT_PREDEF_FILE( "test05.asm" );
    INIT_PREDEF_FILE( "test06.asm" );
    INIT_PREDEF_FILE( "simpx.inc" );
    INIT_PREDEF_FILE( "simple_lib.inc" );
    INIT_PREDEF_FILE( "zstr.inc" );
    INIT_PREDEF_FILE( "math.inc" );
    INIT_PREDEF_FILE( "font-00.asm" );

#if SIMPX_FAKE_FS == 1
    asm4.setSourceFileProvider( std::make_shared<FileSetProvider>( ui->filesList ) );
#else
    asm4.setSourceFileProvider( std::make_shared<Simpleton::FileProviderStd>() );
#endif

#if SIMPX_NO_INI == 0
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
#if SIMPX_NO_INI == 0
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

void applyGamepadKey( Simpleton::mWord &keys, Simpleton::GamePadKeys key, bool flag )
{
    if ( flag )
        keys = keys | key;
}

void MainWindow::on_Timer()
{
    fps.tick();
    statusLabel->setText( QStringLiteral( "fps: %1, clocks: %2" ).arg( fps.getFps() ).arg( simp.getClocks() ) );
    if ( run )
    {
        // set gamepads buttons
        Simpleton::mWord gpKeys = 0;
        for ( auto &gpad: gamepads )
        {
            applyGamepadKey( gpKeys, Simpleton::GPK_LEFT,   gpad.buttonLeft() );
            applyGamepadKey( gpKeys, Simpleton::GPK_RIGHT,  gpad.buttonRight() );
            applyGamepadKey( gpKeys, Simpleton::GPK_UP,     gpad.buttonUp() );
            applyGamepadKey( gpKeys, Simpleton::GPK_DOWN,   gpad.buttonDown() );
            applyGamepadKey( gpKeys, Simpleton::GPK_A,      gpad.buttonA() );
            applyGamepadKey( gpKeys, Simpleton::GPK_B,      gpad.buttonB() );
            applyGamepadKey( gpKeys, Simpleton::GPK_SELECT, gpad.buttonSelect() );
            applyGamepadKey( gpKeys, Simpleton::GPK_START,  gpad.buttonStart() );
            applyGamepadKey( gpKeys, Simpleton::GPK_X,      gpad.buttonX() );
            applyGamepadKey( gpKeys, Simpleton::GPK_Y,      gpad.buttonY() );
            applyGamepadKey( gpKeys, Simpleton::GPK_L1,     gpad.buttonL1() );
            applyGamepadKey( gpKeys, Simpleton::GPK_R1,     gpad.buttonR1() );
            applyGamepadKey( gpKeys, Simpleton::GPK_L2,     gpad.buttonL2() );
            applyGamepadKey( gpKeys, Simpleton::GPK_R2,     gpad.buttonR2() );
            applyGamepadKey( gpKeys, Simpleton::GPK_L3,     gpad.buttonL3() );
            applyGamepadKey( gpKeys, Simpleton::GPK_R3,     gpad.buttonR3() );
        }
        simp.getMMU().setInputWord( 8, gpKeys );

#if SIMPX_SOFT_RENDER == 1
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
#if SIMPX_FAKE_FS == 1
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

void MainWindow::logStr(const QString &str)
{
    ui->debuggerEditor->moveCursor( QTextCursor::End );
    ui->debuggerEditor->insertPlainText( str );
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
        // dump labels to debug window
        std::vector< Simpleton::Identifier > &idens = asm4.getIdentifiers();
        std::sort( idens.begin(), idens.end(),
                   []( const Simpleton::Identifier &a, const Simpleton::Identifier &b )
                    { return a.getValue() < b.getValue(); } );

        ui->debuggerEditor->clear();
        ui->debuggerEditor->moveCursor( QTextCursor::End );
        ui->debuggerEditor->insertPlainText( " === Identifiers ===\n" );
        for ( auto &it: idens )
        {
            if ( it.type != Simpleton::Identifier::Symbol )
                continue;
            ui->debuggerEditor->insertPlainText(
                QString( "%1 %2\n" ).arg( it.getValue(), 4, 16, QLatin1Char( '0' ) ).arg( QString::fromLocal8Bit( it.name.c_str() ) ) );
        }

        run = true;
        ui->msgEdit->setPlainText( "Ok." );
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

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if ( index == 0 )
        ui->glWidget->setFocus();
}

void MainWindow::on_keyInput(bool pressed, int key, int modif )
{
    int usbKey = Simpleton::qtKeyToUSB( key, modif );
    if ( usbKey != 0 )
    {
        simp.getMMU().setInputBit( usbKey - 1, pressed );
    }
}

void MainWindow::on_gamepadsChanged()
{
    // remove disconnected gamepads
    auto it = gamepads.begin();
    while ( it != gamepads.end() )
    {
        if ( !it->isConnected() )
        {
            it = gamepads.erase( it );
            logStr( QStringLiteral( "Gamepad %1 removed.\n" ).arg( it->name() ) );
        }
        else
            it++;
    }
    QList<int> ids = QGamepadManager::instance()->connectedGamepads();
    for ( auto i: ids )
    {
        bool found = false;
        for ( auto &gpad: gamepads )
        {
            if ( gpad.deviceId() == i )
            {
                found = true;
                break;
            }
        }
        if ( !found )
        {
            gamepads.emplace_back( i, nullptr );
            logStr( QStringLiteral( "Gamepad #%1 connected (total: %2)." ).arg( i ).arg( gamepads.size() ) );
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if ( watched == ui->glWidget )
    {
        if ( (event->type() == QEvent::KeyPress) )
        {
            QKeyEvent *keyEvent = static_cast< QKeyEvent * >( event );
            if ( (keyEvent->key() == Qt::Key_Tab) ||
                 (keyEvent->key() == Qt::Key_Backtab) ||
                 (keyEvent->key() == Qt::Key_Alt) )
            {
                ui->glWidget->keyPressEvent( keyEvent );
                return true;
            }
        }
        else if ( (event->type() == QEvent::KeyPress) )
        {
            QKeyEvent *keyEvent = static_cast< QKeyEvent * >( event );
            if ( (keyEvent->key() == Qt::Key_Tab) ||
                 (keyEvent->key() == Qt::Key_Backtab) ||
                 (keyEvent->key() == Qt::Key_Alt) )
            {
                ui->glWidget->keyReleaseEvent( keyEvent );
                return true;
            }
        }
    }
    return QMainWindow::eventFilter( watched, event );
}
