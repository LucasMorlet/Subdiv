#ifndef MATRICE_H
#define MATRICE_H

#include <QString>
#include <vector>
#include <iostream>
//#include <armadillo>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

class Matrice
{

    private :
        unsigned int nb_lignes;
        unsigned int nb_colonnes;
        std::vector <float> coeff;
        //arma::mat matrice_armadillo;
        Eigen::MatrixXd matrice_eigen;
        std::vector < float > point_fixe;
        std::vector < float > tangenteU;
        std::vector < float > tangenteV;

    public :
        // Constructeurs
        Matrice ( unsigned int size );
        Matrice ( unsigned int L, unsigned int C );
        Matrice ( unsigned int L, unsigned int C, std::vector <float> coeff );

        // Accesseurs
        unsigned int getNbLignes ( void );
        unsigned int getNbColonnes ( void );
        //arma::mat getArmaMatrice ( void );
        Eigen::MatrixXd getEigenMatrice ( void );
        void setEigenMatrice ( Eigen::MatrixXd matrice );
        bool isSquare( void ) const;
        double getCoeff ( unsigned int i, unsigned int j ) const;
        void setCoeff ( unsigned int i, unsigned int j, double val ); 
        Matrice getPointFixe ( void ) const;
        Matrice getTangenteU ( void ) const;
        Matrice getTangenteV ( void ) const;

        // ToString
        QString toString ( bool with_holes ) const;

        // Méthodes
        Matrice inverse ( void ) const;
        void extractLeftEigenVectors ( void );
        void normalize ( void );
};

#endif // MATRICE_H
