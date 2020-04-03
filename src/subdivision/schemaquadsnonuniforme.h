#ifndef SCHEMAQUADSNONUNIFORME_H
#define SCHEMAQUADSNONUNIFORME_H

#include "schema.h"

class SchemaQuadsNonUniforme : public Schema
{
    public:
        SchemaQuadsNonUniforme();
        QString getTCS ( void ) const override;

    protected :
        Mot calculMot ( unsigned int precision, double u, double v ) const override;
        void genererMatrices( unsigned int valence_min, unsigned int valence_max ) override;
        void genererDictionnaire ( void ) override;
        void calculCombinaison( unsigned int valence, Mot mot ) override;

        // Les matrices
        virtual Matrice T_0      ( unsigned int k ) const = 0;
        virtual Matrice T_1      ( unsigned int k ) const = 0;
        virtual Matrice T_2      ( unsigned int k ) const = 0;
        virtual Matrice T_3      ( unsigned int k ) const = 0;
        virtual Matrice Identity ( unsigned int k ) const = 0;
};

#endif // SCHEMAQUADSNONUNIFORME_H
