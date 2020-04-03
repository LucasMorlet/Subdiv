#include "vecteur.h"

Vecteur::Vecteur( unsigned int n )
{
    this->taille  = n;
    this->colonne = Eigen::VectorXd(n);
    this->ligne   = Eigen::RowVectorXd(n);
}

Vecteur::Vecteur( const Vecteur& vec )
{
    this->taille  = vec.taille;
    this->colonne = vec.colonne;
    this->ligne   = vec.ligne;
}

Vecteur::Vecteur( const std::vector<float>& coeff )
{
    this->taille  = coeff.size();
    this->colonne = Eigen::VectorXd( this->taille );
    this->ligne   = Eigen::RowVectorXd( this->taille );

    for ( unsigned int i = 0 ; i < this->taille ; i++ )
    {
        this->colonne(i) = this->ligne(i) = coeff[i];
    }
}

void Vecteur::setCoeff ( unsigned int index, float valeur )
{
    this->colonne(index) = this->ligne(index) = valeur;
}

float Vecteur::getCoeff ( unsigned int index ) const
{
    return this->colonne(index);
}

unsigned int Vecteur::getTaille ( void ) const
{
    return this->taille;
}

Eigen::VectorXd Vecteur::getColonne ( void ) const
{
    return this->colonne;
}
Eigen::RowVectorXd Vecteur::getLigne ( void ) const
{
    return this->ligne;
}

void Vecteur::setColonne ( const Eigen::VectorXd& C )
{
    this->colonne = C;
    this->ligne = this->colonne.transpose();
}
void Vecteur::setLigne ( const Eigen::RowVectorXd& L )
{
    this->ligne = L;
    this->colonne = this->ligne.transpose();
}
