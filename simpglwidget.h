#ifndef SIMPGLWIDGET_H
#define SIMPGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>

class SimpGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLShaderProgram prg;
    GLint u_texture;
    GLint a_vertice;
    GLuint texScr = 0;
    QOpenGLFunctions_3_3_Compatibility gl;
    uint16_t *screen = nullptr;

public:
    SimpGLWidget(QWidget*&p): QOpenGLWidget(p) {};
    ~SimpGLWidget();

    void setScreen( uint16_t *data );

signals:
    void painted();

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // SIMPGLWIDGET_H
