#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QString>
#include <QImage>


enum texture_type { color_map, displacement_map, normal_map, bump_map };

class Texture
{
    private :
        GLuint id;
        GLenum num;
        texture_type type;
        QImage image;

    public:
        Texture( QString nom, texture_type t );
};

#endif // TEXTURE_H
