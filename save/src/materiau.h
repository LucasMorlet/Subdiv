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
        Materiau();
        void  sendToShader( GLuint indiceShader ) const;
        // Getters
        float getCoeffAmbiant    ( void );
        float getCoeffDiffus     ( void );
        float getCoeffSpeculaire ( void );
        int   getHardness        ( void );
        // Setters
        void setMateriau ( float amb, float diff, float spec, int hard );
};

#endif // MATERIAU_H
