#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>
#include "objet.h"
#include <GL/glu.h>

class Camera
{
    private :
        float fovy, ratio, znear, zfar;
        glm::mat4 projection;
        float posX, posY, posZ;
        float roll, pitch, yaw;
        glm::mat4 view;
        float sensibilite;

    public :
        Camera( void );
        Camera( float f );
        glm::mat4 getViewMatrix ( void );
        glm::mat4 getProjectionMatrix ( void );
        std::vector <GLfloat> getPosition( void );
        void lookat ( const Objet& obj );
        void CPULookAt ( const Objet& obj );
        void rotate ( int dx, int dy );
        void rotate ( int dx, int dy, const Objet& obj );
        void setRatio ( float r );

    private :
        void calculerProjection ( void );
        void calculerView ( void );

};

#endif // CAMERA_H
