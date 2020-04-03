#ifndef DOOSABIN_H
#define DOOSABIN_H

#include "schemaquads.h"

class DooSabin : public SchemaQuads
{
    public :
        DooSabin(void);
        QString getTES ( void ) const;

        void faces2patches( Objet& obj ) const;

    private :
        Matrice T_0 ( unsigned int k ) const;
        Matrice T_1 ( unsigned int k ) const;
        Matrice T_2 ( unsigned int k ) const;
        Matrice T_3 ( unsigned int k ) const;
        Matrice P_0 ( unsigned int k ) const;
        Matrice P_1 ( unsigned int k ) const;
        Matrice P_2 ( unsigned int k ) const;
        Matrice P_3 ( unsigned int k ) const;
        Matrice Identity ( unsigned int k ) const;
};

#endif // DOOSABIN_H
