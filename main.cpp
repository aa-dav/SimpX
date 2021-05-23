#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat fmt;
    fmt.setRenderableType( QSurfaceFormat::OpenGL );
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CompatibilityProfile );
    fmt.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    QSurfaceFormat::setDefaultFormat( fmt );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
