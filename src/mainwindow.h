#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QLabel>
#include <QSettings>
#include <QListWidget>
#include "utils.h"
#include "simpleton4.h"
#include "simpleton4asm.h"
#include "simpx.h"
#include "fileset.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString lastOpenDir;
    QLabel *statusLabel = nullptr;
    FpsMeter fps;
    Simpleton::SimpX simp;
    Simpleton::Assembler asm4;
    bool run = false;
    QString lastOpenFile;
    QTimer timer;
    //std::shared_ptr< FileSetProvider > files;

    void setViewSize( int coef );

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QListWidgetItem *getFilesSelItem();
    QListWidgetItem *findFilesItem( const QString &name );

    void fileContentReady( const QString &fname, const QByteArray &arr );
    QListWidgetItem *saveCurrentFile(); // save current file and return item of it

    void logStr( const QString &str );

private slots:
    void on_Timer();
    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionView200_triggered();

    void on_actionView300_triggered();

    void on_actionView400_triggered();

    void on_actionStop_triggered();

    void on_actionRun_triggered();

    void on_filesList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_addFileBtn_clicked();

    void on_delFileBtn_clicked();

    void on_fileNameEdit_textEdited(const QString &arg1);

    void on_actionSave_triggered();

    void on_actionImageToAsm_triggered();

    void on_actionResume_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_keyInput( bool pressed, int key, int modif );

private:
    Ui::MainWindow *ui;

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MAINWINDOW_H
