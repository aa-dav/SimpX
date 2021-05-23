#ifndef SIMPGLWIDGET_H
#define SIMPGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

class SimpGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    QOpenGLShaderProgram prg;
    int a_vertice;
    GLint texScr = 0;

public:
    SimpGLWidget(QWidget*&p): QOpenGLWidget(p) {};
    ~SimpGLWidget();

signals:
    void painted();

    // QGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // SIMPGLWIDGET_H
