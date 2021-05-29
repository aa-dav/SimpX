#include "simpglwidget.h"
#include <QMessageBox>

static const char *vertSrc =
    "#version 330\n"
    "const int vid_width = 256; \n"
    "const int vid_height = 192; \n"
    "in vec2 a_vertices; \n"
    "out vec2 v_pixel; \n"
    "void main() \n"
    "{ \n"
    "   vec2 disp = vec2( (a_vertices.x + 1.0) / 2.0, (1.0 - a_vertices.y) / 2.0 ); \n"
    "   v_pixel = vec2( mix( 0, vid_width, disp.x ), mix( 0, vid_height, disp.y ) ); \n"
    "   gl_Position = vec4( a_vertices.x, a_vertices.y, 0.0, 1.0 ); \n"
    "} \n";
static const char *fragSrc =
    "#version 330\n"
    "const int scr_width = 256; \n"
    "const int scr_height = 256; \n"
    "const int char_width = 8; \n"
    "const int char_height = 8; \n"
    "const int map_width = scr_width / char_width; \n"
    "const int map_height = scr_height / char_height; \n"
    "uniform ivec2 u_scrolls; \n"
    "uniform usampler2D u_palette; \n"
    "uniform usampler2D u_bitmap; \n"
    "uniform usampler2D u_charmap; \n"
    "in vec2 v_pixel; \n"
    "out vec3 color; \n"
    "void main() \n"
    "{ \n"
    "   ivec2 pix_xy  = ivec2( ( int( v_pixel.x ) + u_scrolls.x ) % scr_width, \n"
    "                          ( int( v_pixel.y ) + u_scrolls.y ) % scr_height ); \n"
    "   ivec2 char_xy = ivec2( pix_xy.x / char_width, pix_xy.y / char_height ); \n"
    "   ivec2 inch_xy = ivec2( pix_xy.x % char_width, pix_xy.y % char_height ); \n"
    "   int   char_d  = int( texelFetch( u_charmap, char_xy, 0 ).r ); \n"
    "   int   pal_hi  = (char_d >> 8) & 0xF0; \n"
    "         char_d  = char_d & 0x3FF; \n"
    "   ivec2 bmp_xy  = ivec2( (char_d % map_width) * char_width + inch_xy.x, \n"
    "                          (char_d / map_height) * char_height + inch_xy.y ); \n"
    "   ivec2 bmp_nxy = ivec2( bmp_xy.x / 4, bmp_xy.y ); \n"
    "   int   inpx    = bmp_xy.x % 4; \n"
    "   int   pix_d   = int( texelFetch( u_bitmap, bmp_nxy, 0 ).r ); \n"
    "   int   pal_idx = pal_hi + ((pix_d >> (inpx * 4)) & 0xF); \n"
    "   int   pal_d   = int( texelFetch( u_palette, ivec2( pal_idx, 0 ), 0 ).r ); \n"
    "   color = vec3( float((pal_d >> 10) & 0x1F) / 32.0, \n"
    "                 float((pal_d >> 5)  & 0x1F) / 32.0, \n"
    "                 float((pal_d)       & 0x1F) / 32.0 ); \n"
    "} \n";

void SimpGLWidget::set_utex_params( GLuint tex, GLenum slot, int w, int h, uint16_t *ptr )
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
    u_scrolls   = prg.uniformLocation( "u_scrolls" );
    u_palette   = prg.uniformLocation( "u_palette" );
    u_bitmap    = prg.uniformLocation( "u_bitmap" );
    u_charmap   = prg.uniformLocation( "u_charmap" );
    a_vertices  = prg.attributeLocation( "a_vertices" );

    tex_palette = 0;
    gl.glGenTextures( 1, &tex_palette );
    tex_bitmap = 0;
    gl.glGenTextures( 1, &tex_bitmap );
    tex_charmap = 0;
    gl.glGenTextures( 1, &tex_charmap );

    set_utex_params( tex_palette, GL_TEXTURE0, 256, 1, nullptr );
    set_utex_params( tex_bitmap, GL_TEXTURE1, 64, 256, nullptr );
    set_utex_params( tex_charmap, GL_TEXTURE2, 32, 32, nullptr );

    return; // all is ok
error:
    QMessageBox msg;
    msg.setText( prg.log() );
    msg.exec();
}

void SimpGLWidget::setPalette( uint16_t *data )
{
    scr_palette = data;
}

void SimpGLWidget::setBitmap( uint16_t *data )
{
    scr_bitmap = data;
}

void SimpGLWidget::setCharmap( uint16_t *data )
{
    scr_charmap = data;
}

void SimpGLWidget::free_texture( GLuint &tex )
{
    if ( tex != 0 )
    {
        gl.glDeleteTextures( 1, &tex );
        tex = 0;
    }
};

SimpGLWidget::~SimpGLWidget()
{
    free_texture( tex_charmap );
    free_texture( tex_bitmap );
    free_texture( tex_palette );
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
    prg.enableAttributeArray( a_vertices );
    prg.setAttributeArray( a_vertices, verts, 2 );

    //prg.setUniformValue( u_scrolls, QSize( scroll_x, scroll_y ) );
    gl.glUniform2i( u_scrolls, scroll_x, scroll_y );

    prg.setUniformValue( u_palette, 0 );
    set_utex_params( tex_palette, GL_TEXTURE0, 256, 1, scr_palette );
    prg.setUniformValue( u_bitmap, 1 );
    set_utex_params( tex_bitmap, GL_TEXTURE1, 64, 256, scr_bitmap );
    prg.setUniformValue( u_charmap, 2 );
    set_utex_params( tex_charmap, GL_TEXTURE2, 32, 32, scr_charmap );

    gl.glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    prg.disableAttributeArray( a_vertices );

    emit painted();
}
