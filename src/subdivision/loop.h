#ifndef LOOP_H
#define LOOP_H

#include "schematriangles.h"

class Loop : public SchemaTriangles
{
    public :
        Loop ( void );
        void faces2patches( Objet& obj ) const;
        QString getTCS ( void ) const;
        QString getTES ( void ) const;

    private :

        Matrice T_0      ( unsigned int k ) const override;
        Matrice T_1      ( unsigned int k ) const override;
        Matrice T_2      ( unsigned int k ) const override;
        Matrice T_3      ( unsigned int k ) const override;
        Matrice Identity ( unsigned int k ) const override;

        //Matrice P_0 ( unsigned int k ) const override;
        //Matrice P_1 ( unsigned int k ) const override;
        //Matrice P_2 ( unsigned int k ) const override;
        //Matrice P_3 ( unsigned int k ) const override;


};

#endif // LOOP_H
