#ifndef LUMIERE_H
#define LUMIERE_H

#include "shaderprogram.h"

class Lumiere
{
    private :
        glm::vec3 position;
        glm::vec3 couleur;
        unsigned int puissance;

    public :
        // Constructeur
        Lumiere();
        Lumiere
        (
            float x, float y, float z,
            unsigned int r, unsigned int g, unsigned int b,
            unsigned int power
        );

        // Getters
        const glm::vec3& getPosition ( void ) const;
        const glm::vec3& getCouleur  ( void ) const;
        unsigned int getPuissance    ( void ) const;

        // Setters
        void setPosition  ( double x, double y, double z );
        void setCouleur   ( float r, float g, float b );
        void setPuissance ( unsigned int power );

        // Méthodes
        void sendToShader ( GLuint indiceShader, glm::mat4 View, unsigned int i );

        // Méthodes statique
    public :
        static Lumiere Black_Light( void );
};

#endif // LUMIERE_H
