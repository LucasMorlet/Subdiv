#include "camera.h"

Camera::Camera( void )
{
    // Rien, juste pour pouvoir initialiser une caméra vide
}
Camera::Camera( float f )
{
    // Paramètres intrinsèques
    this->fovy  = f;
    this->ratio = 1.0f;
    this->znear = 0.1f;
    this->zfar  = 100.f;
    this->calculerProjection();

    // Paramètres extrinsèques
    this->position = glm::vec3 ( 2.0f, 2.0f, 2.0f );
    this->roll  = 0.0f;
    this->pitch = 0.0f;
    this->yaw   = 0.0f;
    this->calculerView();

    // Autres
    this->sensibilite = 0.01f;
}
const glm::mat4& Camera::getViewMatrix( void ) const
{
    return this->view;
}
const glm::mat4& Camera::getProjectionMatrix( void ) const
{
    return this->projection;
}

void Camera::calculerProjection( void )
{
    this->projection = glm::perspective( glm::radians( this->fovy ), this->ratio, this->znear, this->zfar );
}
void Camera::calculerView( void )
{
    // TODO : vue FPS
    // this->view = glm::
}
void Camera::lookat( const Objet& obj )
{
    this->view = glm::lookAt( this->position, obj.getPosition(), glm::vec3( 0.0, 1.0, 0.0 ) ); // TODO : Vecteur up en fonction des rotations
}
void Camera::CPULookAt ( const Objet& obj )
{
        /*
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    gluPerspective( this->fovy, this->ratio, this->znear, this->zfar );
    glm::vec3 posObj = obj.getPosition();
    gluLookAt( this->position.x, this->position.y, this->position.z, posObj.x, posObj.y, posObj.z, 0.0, 1.0, 0.0 );
    */
}
void Camera::rotate ( int dx, int dy )
{
    this->yaw   += this->sensibilite * 3.141592 * dx;
    if ( this->yaw < 0 )
    {
        this->yaw += 2 * 3.141592;
    }
    else if ( this->yaw > 2 * 3.141592 )
    {
        this->yaw -= 2 * 3.141592;
    }
    this->pitch += this->sensibilite * 3.141592 * dy;
    this->pitch = std::max ( this->pitch, -3.0f * 3.141592f / 8.0f );
    this->pitch = std::min ( this->pitch,  3.0f * 3.141592f / 8.0f );
}

void Camera::rotate ( int dx, int dy, const Objet& obj )
{
    this->rotate( dx, dy );

    glm::vec3 centre = obj.getPosition();
    float rayon = glm::distance ( this->position, centre );
    float y = this->yaw   + 3.141592f;
    float p = this->pitch + 3.141592f;
    // float r = this->roll  + 3.141592f;

    this->position.x = centre[0] + rayon * cos(y) * cos(p);
    this->position.y = centre[1] + rayon * sin(p);
    this->position.z = centre[2] + rayon * sin(y) * cos(p);

}

void Camera::setRatio( float r )
{
    this->ratio = r;
    this->calculerProjection();
}
