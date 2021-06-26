#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include "simpx.h"

int main(int argc, char *argv[])
{
#if PPU_SOFT_RENDER != 1
    QSurfaceFormat fmt;
    fmt.setRenderableType( QSurfaceFormat::OpenGL );
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CompatibilityProfile );
    fmt.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    QSurfaceFormat::setDefaultFormat( fmt );
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
