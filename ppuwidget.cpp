#include "ppuwidget.h"
#include <QMessageBox>

#if PPU_SOFT_RENDER == 1

#include <QPainter>

PPUWidget::PPUWidget(QWidget *parent) : QWidget(parent),
    image( 256, 192, QImage::Format_RGB555 )
{
}


void PPUWidget::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter( this );
    QRect rectFrom = { 0, 0, image.width(), image.height() };
    QRect rectTo = { 0, 0, width(), height() };
    painter.drawImage( rectTo, image, rectFrom );
    emit painted();
}

void PPUWidget::setBitmap( uint16_t *data )
{
    memcpy( image.bits(), data, 256 * 192 * 2 );
}

#else

static const char *vertSrc = R"str(
    #version 330
    const int vid_width = 256;
    const int vid_height = 192;
    in vec2 a_vertices;
    out vec2 v_pixel;
    void main()
    {
       vec2 disp = vec2( (a_vertices.x + 1.0) / 2.0, (1.0 - a_vertices.y) / 2.0 );
       v_pixel = vec2( mix( 0, vid_width, disp.x ), mix( 0, vid_height, disp.y ) );
       gl_Position = vec4( a_vertices.x, a_vertices.y, 0.0, 1.0 );
    }
)str";

static const char *fragSrc = R"str(
    #version 330
    uniform usampler2D u_bitmap;
    in vec2 v_pixel;
    out vec3 color;
    void main()
    {
       ivec2 xy = ivec2( int( v_pixel.x ), int( v_pixel.y ) );
       int   pix   = int( texelFetch( u_bitmap, xy, 0 ).r );
       color = vec3( float((pix >> 10) & 0x1F) / 32.0,
                     float((pix >> 5)  & 0x1F) / 32.0,
                     float((pix)       & 0x1F) / 32.0 );
    }
)str";

void PPUWidget::set_utex_params( GLuint tex, GLenum slot, int w, int h, uint16_t *ptr )
{
    gl.glActiveTexture( slot );
    gl.glBindTexture( GL_TEXTURE_2D, tex );
    if ( ptr == nullptr )
        gl.glTexImage2D( GL_TEXTURE_2D, 0, GL_R16UI, w, h, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr );
    else
        gl.glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RED_INTEGER, GL_UNSIGNED_SHORT, ptr );

    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    gl.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}

void PPUWidget::initializeGL()
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
    u_bitmap    = prg.uniformLocation( "u_bitmap" );
    a_vertices  = prg.attributeLocation( "a_vertices" );
    tex_bitmap = 0;
    gl.glGenTextures( 1, &tex_bitmap );

    set_utex_params( tex_bitmap, GL_TEXTURE0, 256, 192, nullptr );

    return; // all is ok
error:
    QMessageBox msg;
    msg.setText( prg.log() );
    msg.exec();
}

void PPUWidget::setBitmap( uint16_t *data )
{
    if ( tex_bitmap )
        set_utex_params( tex_bitmap, GL_TEXTURE0, 256, 192, data );
}

void PPUWidget::free_texture( GLuint &tex )
{
    if ( tex != 0 )
    {
        gl.glDeleteTextures( 1, &tex );
        tex = 0;
    }
};

PPUWidget::~PPUWidget()
{
    free_texture( tex_bitmap );
}

void PPUWidget::resizeGL(int w, int h)
{
    gl.glViewport( 0, 0, w, h );
}

void PPUWidget::paintGL()
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
    prg.enableAttributeArray( a_vertices );
    prg.setAttributeArray( a_vertices, verts, 2 );

    prg.setUniformValue( u_bitmap, 0 );

    gl.glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    prg.disableAttributeArray( a_vertices );

    emit painted();
}

#endif // ifdef PPU_SOFT_RENDER
