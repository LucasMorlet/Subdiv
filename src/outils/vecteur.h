#ifndef VECTEUR_H
#define VECTEUR_H

#include <Eigen/Eigenvalues>
#include <vector>

class Vecteur
{
    private :
        unsigned int taille;
        Eigen::VectorXd colonne;
        Eigen::RowVectorXd ligne;

    public :
        Vecteur( unsigned int n );
        Vecteur( const Vecteur& vec );
        Vecteur( const std::vector<float>& coeff );

        void setCoeff ( unsigned int index, float valeur );
        float getCoeff ( unsigned int index ) const;
        unsigned int getTaille ( void ) const;
        Eigen::VectorXd getColonne ( void ) const;
        Eigen::RowVectorXd getLigne ( void ) const;
        void setColonne ( const Eigen::VectorXd& C );
        void setLigne ( const Eigen::RowVectorXd& L );
};

#endif // VECTEUR_H
