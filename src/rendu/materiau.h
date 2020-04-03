#ifndef MATERIAU_H
#define MATERIAU_H

#include "shaderprogram.h"

class Materiau
{
    private :
        float coeff_ambiant;
        float coeff_speculaire;
        float coeff_diffus;
        int hardness;

    public :
        // Constructeur
        Materiau();

        // Getters
        float getCoeffAmbiant    ( void ) const;
        float getCoeffDiffus     ( void ) const;
        float getCoeffSpeculaire ( void ) const;
        int   getHardness        ( void ) const;

        // Setters
        void setMateriau ( float amb, float diff, float spec, int hard );

        // Méthodes
        void sendToShader( GLuint indiceShader ) const;
};

#endif // MATERIAU_H
