#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

class Shader
{
    private :
        GLenum type;
        QString path;
        GLuint indice;

    public:
        Shader ( void );
        Shader ( GLenum t, QString p, QString* erreur );
        ~Shader( void );
        GLuint getIndice ( void ) const;

};

#endif // SHADER_H
