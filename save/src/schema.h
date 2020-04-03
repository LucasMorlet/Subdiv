#ifndef SCHEMA_H
#define SCHEMA_H

#include "objet.h"
#include "matrice.h"
#include "mot.h"
#include "face.h"
#include <QString>

class Schema
{
    protected :

        unsigned int valence_ordinaire;
        std::vector < std::vector<Matrice> > liste_matrices;
        std::vector<float> liste_coeff;
        QString nom;

    public :

        Schema();
        virtual void genererCombinaisons ( unsigned int valence_min, unsigned int valence_max ) = 0;
        void lireCombinaisons ( QString path );
        void ecrireCombinaisons ( QString path ) const;
        void envoiShader( void ) const;
        QString getNom ( void ) const;

        virtual QString getTCS ( void ) const = 0;
        virtual QString getTES ( void ) const = 0;
        virtual void faces2patches( Objet& obj ) const = 0;

    protected :

        void calculCombinaison( unsigned int valence, Mot mot );
        Matrice& getMatrice ( unsigned int nom, unsigned int valence );        

        virtual Mot calculMot ( unsigned int precision, double u, double v ) const = 0;
        virtual void genererMatrices ( unsigned int valence_min, unsigned int valence_max ) = 0;

        virtual Matrice T_0 ( unsigned int k ) const = 0;
        virtual Matrice T_1 ( unsigned int k ) const = 0;
        virtual Matrice T_2 ( unsigned int k ) const = 0;
        virtual Matrice T_3 ( unsigned int k ) const = 0;
        virtual Matrice P_0 ( unsigned int k ) const = 0;
        virtual Matrice P_1 ( unsigned int k ) const = 0;
        virtual Matrice P_2 ( unsigned int k ) const = 0;
        virtual Matrice P_3 ( unsigned int k ) const = 0;
        virtual Matrice Identity ( unsigned int k ) const = 0;
};

#endif // SCHEMA_H

