#ifndef CATMULLCLARK_H
#define CATMULLCLARK_H

#include "schemaquads.h"

class CatmullClark : public SchemaQuads
{
    public :
        CatmullClark( void );
        QString getTES ( void ) const override;
        void faces2patches( Objet& obj ) const override;

    private :
        Matrice T_0 ( unsigned int k ) const override;
        Matrice T_1 ( unsigned int k ) const override;
        Matrice T_2 ( unsigned int k ) const override;
        Matrice T_3 ( unsigned int k ) const override;

        /*
        Matrice P_0 ( unsigned int k ) const override;
        Matrice P_1 ( unsigned int k ) const override;
        Matrice P_2 ( unsigned int k ) const override;
        Matrice P_3 ( unsigned int k ) const override;
        */

        Matrice Identity( unsigned int k ) const override;
};

#endif // CATMULLCLARK_H
