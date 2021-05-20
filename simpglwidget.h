#ifndef SIMPGLWIDGET_H
#define SIMPGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

class SimpGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLShaderProgram prg;
    int a_vertice, a_color;

public:
    SimpGLWidget(QWidget*&p): QOpenGLWidget(p) {};

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // SIMPGLWIDGET_H
