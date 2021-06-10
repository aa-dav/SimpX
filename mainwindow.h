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
    std::shared_ptr< FileSetProvider > files;

    void setViewSize( int coef );

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int getFilesSelRow();
    QListWidgetItem *getFilesSelItem();
    void setFilesSelRow( int row );

    void fileContentReady( const QString &fname, const QByteArray &arr );
    void saveCurrentFile();

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
