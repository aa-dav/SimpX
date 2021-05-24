#include "simpglwidget.h"
#include <QMessageBox>

static const char *vertSrc =
    "#version 330\n"
    "in vec2 a_vertice; \n"
    "out vec3 v_color; \n"
    "out vec2 v_pixel; \n"
    "void main() \n"
    "{ \n"
    "   vec2 disp = vec2( (a_vertice.x + 1.0) / 2.0, (1.0 - a_vertice.y) / 2.0 ); \n"
    "   v_color = vec3( disp.x, disp.y, 0.0 ); \n"
    "   v_pixel = vec2( mix( 0, 255, disp.x ), mix( 0, 255, disp.y ) ); \n"
    "   gl_Position = vec4( a_vertice.x, a_vertice.y, 0.0, 1.0 ); \n"
    "} \n";
static const char *fragSrc =
    "#version 330\n"
    "uniform usampler2D u_texture; \n"
    "in vec3 v_color; \n"
    "in vec2 v_pixel; \n"
    "out vec3 color; \n"
    "void main() \n"
    "{ \n"
    "   ivec2 pix = ivec2( int( v_pixel.x / 16 ), int( v_pixel.y ) ); \n"
    "   int wrd = int( texelFetch( u_texture, pix, 0 ).r ); \n"
    "   int bit = ((wrd << (int( v_pixel.x ) % 16)) & 0x8000); \n"
    "   if ( bit == 0 ) \n"
    "   { \n"
    "       color = vec3( 0.0, 0.0, 0.0 ); \n"
    "   } \n"
    "   else \n"
    "   { \n"
    "       color = vec3( 1.0, 1.0, 1.0 ); \n"
    "   } \n"
    "} \n";

void SimpGLWidget::initializeGL()
{
    bool res;

    gl.initializeOpenGLFunctions();

    gl.glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );

    res = prg.addShaderFromSourceCode( QOpenGLShader::Vertex, vertSrc );
    if ( !res )
        goto error;
    res = prg.addShaderFromSourceCode( QOpenGLShader::Fragment, fragSrc );
    if ( !res )
        goto error;
    res = prg.link();
    if ( !res )
        goto error;
    u_texture   = prg.uniformLocation( "u_texture" );
    a_vertice   = prg.attributeLocation( "a_vertice" );
    texScr = 0;
    gl.glGenTextures( 1, &texScr );

    gl.glActiveTexture( GL_TEXTURE0 );
    gl.glBindTexture( GL_TEXTURE_2D, texScr );
    gl.glTexImage2D( GL_TEXTURE_2D, 0, GL_R16UI, 16, 256, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr );
    gl.glBindTexture( GL_TEXTURE_2D, 0 );

    return; // all is ok
error:
    QMessageBox msg;
    msg.setText( prg.log() );
    msg.exec();
}

void SimpGLWidget::setScreen( uint16_t *data )
{
    screen = data;
}

SimpGLWidget::~SimpGLWidget()
{
    if ( texScr != 0 )
    {
        gl.glDeleteTextures( 1, &texScr );
        texScr = 0;
    }
}

void SimpGLWidget::resizeGL(int w, int h)
{
    gl.glViewport( 0, 0, w, h );
}

void SimpGLWidget::paintGL()
{
    static GLfloat x = 0.0f;
    x += 0.01f;
    if ( x > 1.0f )
        x = 0.0f;
    static GLfloat verts[ 2 * 4 ] =
    {
        -1.0f, +1.0f,
        +1.0f, +1.0f,
        -1.0f, -1.0f,
        +1.0f, -1.0f
    };
    prg.bind();
    prg.enableAttributeArray( a_vertice );
    prg.setAttributeArray( a_vertice, verts, 2 );

    prg.setUniformValue( u_texture, 0 );
    gl.glActiveTexture( GL_TEXTURE0 );
    gl.glBindTexture( GL_TEXTURE_2D, texScr );
    if ( screen )
        gl.glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 16, 256, GL_RED_INTEGER, GL_UNSIGNED_SHORT, screen );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    gl.glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    prg.disableAttributeArray( a_vertice );

    emit painted();
}
