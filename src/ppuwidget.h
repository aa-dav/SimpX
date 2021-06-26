#ifndef PPUWIDGET_H
#define PPUWIDGET_H

#include "simpx.h"

#if PPU_SOFT_RENDER == 1

#include "QWidget"

class PPUWidget : public QWidget
{
    Q_OBJECT

    QImage image;

public:
    explicit PPUWidget(QWidget *parent = nullptr);
    QImage &getImage() { return image; };

signals:
    void keyInput( bool pressed, int keyCode, int modif );

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
public:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#else

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>

class PPUWidget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLShaderProgram prg;
    GLint u_bitmap;
    GLuint tex_bitmap = 0;
    GLint a_vertices;
    QOpenGLFunctions_3_3_Compatibility gl;
    QImage image;

    void free_texture( GLuint &tex );
    void set_utex_params( GLuint tex, GLenum slot, int w, int h, uint16_t *ptr );

public:
    PPUWidget(QWidget*&p): QOpenGLWidget(p),
        image( 256, 192, QImage::Format_RGB555 ) {};
    ~PPUWidget();
    QImage &getImage() { return image; };

signals:
    void keyInput( bool pressed, int keyCode, int modif );

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // QWidget interface
public:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif

#endif // PPUWIDGET_H
