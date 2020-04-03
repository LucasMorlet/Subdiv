#ifndef SCHEMATRIANGLES_H
#define SCHEMATRIANGLES_H

#include "schema.h"

class SchemaTriangles : public Schema
{
    public :
        SchemaTriangles();       
        QString getTCS ( void ) const;
        //void genererCombinaisons ( unsigned int valence_min, unsigned int valence_max );

    protected :
        Mot calculMot ( unsigned int precision, double u, double v ) const;
        void genererMatrices( unsigned int valence_min, unsigned int valence_max );
        void genererDictionnaire ( void ) override;
        void calculCombinaison( unsigned int valence, Mot mot ) override;

        // Les matrices
        virtual Matrice T_0      ( unsigned int k ) const = 0;
        virtual Matrice T_1      ( unsigned int k ) const = 0;
        virtual Matrice T_2      ( unsigned int k ) const = 0;
        virtual Matrice T_3      ( unsigned int k ) const = 0;
        virtual Matrice Identity ( unsigned int k ) const = 0;
};

#endif // SCHEMATRIANGLES_H
