#include "simpglwidget.h"
#include <QMessageBox>

static const char *vertSrc =
    "#version 330\n"
    "in vec2 a_vertice; \n"
    "in vec3 a_color; \n"
    "out vec3 v_color; \n"
    "void main() \n"
    "{ \n"
    "   v_color = a_color; \n"
    "   gl_Position = vec4( a_vertice.x, a_vertice.y, 0.0, 1.0 ); \n"
    "} \n";
static const char *fragSrc =
    "#version 330\n"
    "in vec3 v_color; \n"
    "out vec3 color; \n"
    "void main() \n"
    "{ \n"
    "   color = v_color; \n"
    "} \n";

void SimpGLWidget::initializeGL()
{
    bool res;
    glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
    res = prg.addShaderFromSourceCode( QOpenGLShader::Vertex, vertSrc );
    if ( !res )
        goto error;
    res = prg.addShaderFromSourceCode( QOpenGLShader::Fragment, fragSrc );
    if ( !res )
        goto error;
    res = prg.link();
    if ( !res )
        goto error;
    a_vertice   = prg.attributeLocation( "a_vertice" );
    a_color     =  prg.attributeLocation( "a_color" );
    return; // all is ok
error:
    QMessageBox msg;
    msg.setText( prg.log() );
    msg.exec();
}

void SimpGLWidget::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, h );
}

void SimpGLWidget::paintGL()
{
    static GLfloat x = 0.0f;
    x += 0.01f;
    if ( x > 1.0f )
        x = 0.0f;
    GLfloat verts[ 2 * 3 ] = { -1.0f, 1.0f, +1.0f, 1.0f, 0.0f, -1.0f };
    GLfloat colors[ 3 * 3 ] = { 0, 0, 1, 1, 0, 0, 0, 1, x };
    prg.bind();
    prg.enableAttributeArray( a_vertice );
    prg.setAttributeArray( a_vertice, verts, 2 );
    prg.enableAttributeArray( a_color );
    prg.setAttributeArray( a_color, colors, 3 );

    glDrawArrays( GL_TRIANGLES, 0, 3 );

    prg.disableAttributeArray( a_vertice );
    prg.disableAttributeArray( a_color );
}
