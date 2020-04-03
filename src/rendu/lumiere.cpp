#include "lumiere.h"

Lumiere::Lumiere()
{
    this->position  = glm::vec3 ( 0.0f, 0.0f, 0.0f );
    this->couleur   = glm::vec3 ( 1.0f, 1.0f, 1.0f );
    this->puissance = 40;
}

Lumiere::Lumiere( float x, float y, float z, unsigned int r, unsigned int g, unsigned int b, unsigned int power )
{
    this->position  = glm::vec3 ( x, y, z );
    this->couleur   = glm::vec3 ( r, g, b );
    this->puissance = power;
}

void Lumiere::sendToShader( GLuint indiceShader, glm::mat4 View, unsigned int i )
{
    // On replace la lumière dans le repère caméra
    glm::vec4 position_camera = glm::vec4( this->position, 1.0f);
    position_camera = View*position_camera;

    // View Position
    QString position = QString ( "lesLumieres[");
    position.push_back( QString::number(i) );
    position.push_back("].position");
    GLuint loc = glGetUniformLocation( indiceShader, position.toStdString().c_str() );
    glUniform3f ( loc, position_camera.x, position_camera.y, position_camera.z );

    // Couleur
    QString couleur = QString ( "lesLumieres[");
    couleur.push_back( QString::number(i) );
    couleur.push_back("].couleur");
    loc = glGetUniformLocation( indiceShader, couleur.toStdString().c_str() );
    glUniform3f ( loc, this->couleur.r, this->couleur.g, this->couleur.b );

    // Puissance
    QString puissance = QString ( "lesLumieres[");
    puissance.push_back( QString::number(i) );
    puissance.push_back("].puissance");
    loc = glGetUniformLocation( indiceShader, puissance.toStdString().c_str() );
    glUniform1i ( loc, this->puissance );
}

void Lumiere::setPosition( double x, double y, double z )
{
    this->position = glm::vec3 ( x, y, z );
}

void Lumiere::setCouleur( float r, float g, float b )
{
    this->couleur = glm::vec3 ( r, g, b );
}

const glm::vec3& Lumiere::getPosition( void ) const
{
    return this->position;
}

const glm::vec3& Lumiere::getCouleur( void ) const
{
    return this->couleur;
}

unsigned int Lumiere::getPuissance( void ) const
{
    return this->puissance;
}

void Lumiere::setPuissance( unsigned int power )
{
    this->puissance = power;
}

Lumiere Lumiere::Black_Light( void )
{
    return Lumiere ( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0 );
}
