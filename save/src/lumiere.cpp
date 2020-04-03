#include "lumiere.h"

Lumiere::Lumiere()
{
    this->posX  = 0.0f;
    this->posY  = 0.0f;
    this->posZ  = 0.0f;
    this->rouge = 255;
    this->vert  = 255;
    this->bleu  = 255;
    this->puissance = 40;
}

Lumiere::Lumiere( float x, float y, float z, unsigned int r, unsigned int g, unsigned int b, unsigned int power )
{
    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->rouge = r;
    this->vert = g;
    this->bleu = b;
    this->puissance = power;
}

void Lumiere::sendToShader( GLuint indiceShader, glm::mat4 View, unsigned int i )
{
    // On replace la lumière dans le repère caméra
    glm::vec4 vec4_position = glm::vec4( this->posX, this->posY, this->posZ, 1.0f);
    vec4_position = View*vec4_position;

    // View Position
    QString position = QString ( "lesLumieres[");
    position.push_back( QString::number(i) );
    position.push_back("].position");
    GLuint loc = glGetUniformLocation( indiceShader, position.toStdString().c_str() );
    glUniform3f ( loc, vec4_position.x, vec4_position.y, vec4_position.z );

    // Couleur
    QString couleur = QString ( "lesLumieres[");
    couleur.push_back( QString::number(i) );
    couleur.push_back("].couleur");
    loc = glGetUniformLocation( indiceShader, couleur.toStdString().c_str() );
    glUniform3f ( loc, this->rouge/255.0f, this->vert/255.0f, this->bleu/255.0f );

    // Puissance
    QString puissance = QString ( "lesLumieres[");
    puissance.push_back( QString::number(i) );
    puissance.push_back("].puissance");
    loc = glGetUniformLocation( indiceShader, puissance.toStdString().c_str() );
    glUniform1i ( loc, this->puissance );
}

void Lumiere::setPosition( double x, double y, double z )
{
    this->posX = x;
    this->posY = y;
    this->posZ = z;
}

void Lumiere::setCouleur( unsigned int r, unsigned int g, unsigned int b )
{
    this->rouge = r;
    this->vert  = g;
    this->bleu  = b;
}

float Lumiere::getPosX( void )
{
    return this->posX;
}

float Lumiere::getPosY( void )
{
    return this->posY;
}

float Lumiere::getPosZ( void )
{
    return this->posZ;
}

unsigned int Lumiere::getRouge( void )
{
    return this->rouge;
}

unsigned int Lumiere::getVert( void )
{
    return this->vert;
}

unsigned int Lumiere::getBleu( void )
{
    return this->bleu;
}

unsigned int Lumiere::getPuissance( void )
{
    return this->puissance;
}

void Lumiere::setPuissance( unsigned int power )
{
    this->puissance = power;
}

Lumiere Lumiere::Black_Light( void )
{
    return Lumiere ( 0.0, 0.0, 0.0, 0, 0, 0, 0 );
}
