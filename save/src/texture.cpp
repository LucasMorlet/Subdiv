#include "texture.h"

Texture::Texture( QString nom, texture_type t )
{
    // Création de l'objet texture
    this->image = QGLWidget::convertToGLFormat( QImage(QString( nom )) );
    this->type = t;
    this->num = GL_TEXTURE0 + t;
    /*
    switch ( t )
    {
        case color_map        : this->num = GL_TEXTURE0; break;
        case displacement_map : this->num = GL_TEXTURE1; break;
        case normal_map       : this->num = GL_TEXTURE2; break;
        case bump_map         : this->num = GL_TEXTURE3; break;
        default               : this->num = GL_TEXTURE0; break;
    }
    */

    // Copie de la texture dans OpenGL
    glEnable(GL_TEXTURE_2D);

        glGenTextures( 1, &(this->id) );
        glBindTexture( GL_TEXTURE_2D, this->id );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->image.width(), this->image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image.bits() );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // comment on interprète l'inter-texel
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // comment on interprète un groupe de texels

    glDisable(GL_TEXTURE_2D);
}

