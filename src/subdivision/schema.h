#ifndef SCHEMA_H
#define SCHEMA_H

#include "../rendu/objet.h"
#include "../outils/operateurs.h"
#include "mot.h"

#include <QString>
//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GL/wglew.h>

class Schema
{
    protected :

        unsigned int valence_ordinaire;
        std::vector < std::vector<Matrice> > liste_matrices;
        std::vector < Mot > dictionnaire;

        std::vector<float> liste_coeff;
        std::vector<float> liste_coeff_u;
        std::vector<float> liste_coeff_v;
        QString nom;

    public :

        Schema();
        void genererCombinaisons ( unsigned int valence_min, unsigned int valence_max );

        void ecrireCombinaisons ( QString path ) const;
        void ecrireTangentesU ( QString path ) const;
        void ecrireTangentesV ( QString path ) const;

        void lireCombinaisons ( QString path );
        void lireTangentesU ( QString path );
        void lireTangentesV ( QString path );

        void envoiShader( void ) const;
        QString getNom ( void ) const;

        virtual QString getTCS ( void ) const = 0;
        virtual QString getTES ( void ) const = 0;
        virtual void faces2patches( Objet& obj ) const = 0;

    protected :

        virtual void calculCombinaison( unsigned int valence, Mot mot ) = 0;
        const Matrice& getMatrice ( unsigned int nom, unsigned int valence ) const;

        virtual void genererMatrices ( unsigned int valence_min, unsigned int valence_max ) = 0;
        virtual void genererDictionnaire ( void ) = 0;
        virtual Mot calculMot ( unsigned int precision, double u, double v ) const = 0;

};

#endif // SCHEMA_H

