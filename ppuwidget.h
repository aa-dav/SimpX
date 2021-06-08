#ifndef PPUWIDGET_H
#define PPUWIDGET_H

#define PPU_SOFT_RENDER 1

#if PPU_SOFT_RENDER == 1

#include "QWidget"

class PPUWidget : public QWidget
{
    Q_OBJECT

    QImage image;

public:
    explicit PPUWidget(QWidget *parent = nullptr);
    QImage &getImage() { return image; };

    void setBitmap( uint16_t *data );

signals:
    void painted();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
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

    void free_texture( GLuint &tex );
    void set_utex_params( GLuint tex, GLenum slot, int w, int h, uint16_t *ptr );

public:
    PPUWidget(QWidget*&p): QOpenGLWidget(p) {};
    ~PPUWidget();

    void setBitmap( uint16_t *data );

signals:
    void painted();

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif

#endif // PPUWIDGET_H
