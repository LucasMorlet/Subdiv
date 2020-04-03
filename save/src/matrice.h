#ifndef MATRICE_H
#define MATRICE_H

#include "shaderprogram.h"
#include <QString>
#include <boost/numeric/ublas/matrix.hpp> //TODO ce serait cool de remplacer boost par glm

class Matrice
{

    private :
        unsigned int nb_lignes;
        unsigned int nb_colonnes;
        boost::numeric::ublas::matrix < double > M;

    public :
        // Constructeur
        Matrice ( unsigned int size );
        Matrice ( unsigned int L, unsigned int C );
        Matrice ( unsigned int L, unsigned int C, boost::numeric::ublas::matrix < double > mat );
        // Accesseurs
        unsigned int getNbLignes ( void );
        unsigned int getNbColonnes ( void );
        boost::numeric::ublas::matrix < double > getMatrice ( void );
        // Accesseurs des coefficients de la matrice
        double getCoeff ( unsigned int i, unsigned int j ); // const;
        void setCoeff ( unsigned int i, unsigned int j, double val );
        void normalize ( void );
        // ToString
        QString toString ( bool with_holes );
        // Les communications avec le shader
        void passToShader ( ShaderProgram* progShad, const QString& str ); // const;
};

#endif // MATRICE_H

Matrice operator* ( Matrice A, Matrice B );
