#ifndef MATRICE_H
#define MATRICE_H

#include <QString>
#include <vector>
#include <iostream>
//#include <armadillo>
//#include <eigen3/Eigen/Dense>
//#include <eigen3/Eigen/Eigenvalues>

class Matrice
{

    private :
        unsigned int nb_lignes;
        unsigned int nb_colonnes;
        std::vector <float> coeff;
        //arma::mat matrice_armadillo;
        //Eigen::MatrixXd matrice_eigen;
        std::vector < float > point_fixe;
        std::vector < float > tangenteU;
        std::vector < float > tangenteV;

    public :
        // Constructeurs
        Matrice ( unsigned int size );
        Matrice ( unsigned int L, unsigned int C );
        Matrice ( unsigned int L, unsigned int C, std::vector <float> coeff );
        //Matrice (unsigned int L, unsigned int C, arma::mat mat_arma, Eigen::MatrixXd mat_eig );

        // Accesseurs
        unsigned int getNbLignes ( void );
        unsigned int getNbColonnes ( void );
        //arma::mat getArmaMatrice ( void );
        //Eigen::MatrixXd getEigenMatrice ( void );
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

Matrice operator* (   Matrice A, Matrice B );
Matrice operator* ( float coeff, Matrice M );
Matrice operator* ( Matrice M, float coeff );
