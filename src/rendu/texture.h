#ifndef TEXTURE_H
#define TEXTURE_H

#include "shaderprogram.h"

//#include <GL/glew.h>
#include <QGLWidget>
#include <QString>
#include <QImage>
#include <QCoreApplication>

// #include <QLabel> // pour le test de texture

enum texture_type { color, normal };

class Texture
{
    private :
        GLuint id;
        GLenum num;
        texture_type type;
        QImage image;

    public:
        Texture ( void );
        Texture ( QString nom, texture_type t );
        ~Texture ( void );
        void sendToShader ( GLuint indiceShader ) const;
        void sendToMemory ( void );
};

#endif // TEXTURE_H
