#ifndef LUMIERE_H
#define LUMIERE_H

#include "shaderprogram.h"

class Lumiere
{
    private :
        float posX, posY, posZ;
        unsigned int rouge, vert, bleu;
        unsigned int puissance;

    public:
        Lumiere();
        Lumiere
        (
            float x, float y, float z,
            unsigned int r, unsigned int g, unsigned int b,
            unsigned int power
        );
        void sendToShader ( GLuint indiceShader, glm::mat4 View, unsigned int i );

        void setPosition( double x, double y, double z );
        float getPosX ( void );
        float getPosY ( void );
        float getPosZ ( void );

        void setCouleur ( unsigned int r, unsigned int g, unsigned int b );
        unsigned int getRouge ( void );
        unsigned int getVert  ( void );
        unsigned int getBleu  ( void );

        unsigned int getPuissance ( void );
        void  setPuissance ( unsigned int power );

    public :
        static Lumiere Black_Light( void );
};

#endif // LUMIERE_H
