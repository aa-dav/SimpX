#ifndef SIMPGLWIDGET_H
#define SIMPGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>

class SimpGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLShaderProgram prg;
    GLint u_scrolls;
    GLint u_palette;
    GLint u_bitmap;
    GLint u_charmap;
    GLint a_vertices;
    GLuint tex_palette = 0;
    GLuint tex_bitmap = 0;
    GLuint tex_charmap = 0;
    QOpenGLFunctions_3_3_Compatibility gl;

    uint16_t scroll_x = 0, scroll_y = 0;
    uint16_t *scr_palette = nullptr;
    uint16_t *scr_bitmap = nullptr;
    uint16_t *scr_charmap = nullptr;

    void free_texture( GLuint &tex );
    void set_utex_params( GLuint tex, GLenum slot, int w, int h, uint16_t *ptr );

public:
    SimpGLWidget(QWidget*&p): QOpenGLWidget(p) {};
    ~SimpGLWidget();

    void setScrolls( uint16_t x, uint16_t y )
    {
        scroll_x = x;
        scroll_y = y;
    };
    void setPalette( uint16_t *data );
    void setBitmap( uint16_t *data );
    void setCharmap( uint16_t *data );

signals:
    void painted();

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // SIMPGLWIDGET_H
