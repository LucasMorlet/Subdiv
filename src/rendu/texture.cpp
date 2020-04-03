#include "texture.h"

Texture::Texture( void )
{
    // Rien
}

Texture::Texture( QString nom, texture_type t )
{
    // Création de l'objet texture
    this->image = QGLWidget::convertToGLFormat( QImage(QString( nom )) );
    this->type = t;
    this->num = GL_TEXTURE0 + t;

    this->sendToMemory();

    /* Test si l'image est bien chargée *
    QLabel* label = new QLabel( NULL );
    QPixmap pixmap;
    pixmap.convertFromImage( this->image );
    label->setPixmap(pixmap);
    label->resize(pixmap.size());
    label->show();
    // Fin test */
}

void Texture::sendToMemory ( void )
{
    glEnable(GL_TEXTURE_2D);

        glGenTextures( 1, &(this->id) );
        glBindTexture( GL_TEXTURE_2D, this->id );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->image.width(), this->image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image.bits() );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // comment on interprète l'inter-texel
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // comment on interprète un groupe de texels

    glDisable(GL_TEXTURE_2D);
}

Texture::~Texture ( void )
{
    // On doit supprimer les affectations OpenGL avant de supprimer la texture
}

void Texture::sendToShader( GLuint indiceShader ) const
{
    GLint localisation; // La localisation est toujours la même, il doit y avoir moyen de la fixer
    switch ( this->type )
    {
        case color        : localisation = glGetUniformLocation( indiceShader, "color_map" ); break;
        case normal       : localisation = glGetUniformLocation( indiceShader, "normal_map" ); break;
        default           : localisation = glGetUniformLocation( indiceShader, "color_map" ); break;
    }
    glUniform1i ( localisation, this->type );
    glActiveTexture ( this-> num );
    glBindTexture( GL_TEXTURE_2D, this->id );
}
