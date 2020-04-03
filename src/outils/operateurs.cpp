#include "operateurs.h"

Matrice operator* ( Matrice A, Matrice B )
{
    if ( A.getNbColonnes() != B.getNbLignes() )
    {
        std::cout << "Erreur : les matrices ne sont pas compatibles par produit" << std::endl;
        return Matrice( 0, 0 );
    }
    else
    {
        Matrice m ( A.getNbLignes(), B.getNbColonnes() );
        m.setEigenMatrice ( A.getEigenMatrice() * B.getEigenMatrice() );
        return m;
    }
}

Matrice operator* ( float lambda, Matrice M )
{
    Matrice res = Matrice ( M.getNbLignes(), M.getNbColonnes() );
    for ( unsigned int i = 0 ; i < M.getNbLignes() ; i++ )
    {
        for ( unsigned int j = 0 ; j < M.getNbColonnes() ; j++ )
        {
            res.setCoeff( i, j, lambda * M.getCoeff( i, j ) );
        }
    }
    return res;
}

Vecteur operator* ( float lambda, Vecteur V )
{
    unsigned int n = V.getTaille();
    Vecteur res = Vecteur ( n );
    for ( unsigned int i = 0 ; i < n ; i++ )
    {
        res.setCoeff( i, lambda * V.getCoeff(i) );
    }
    return res;
}

Vecteur operator* ( Matrice M, Vecteur V )
{
    unsigned int n = V.getTaille();
    Vecteur res = Vecteur ( n );
    if ( n != M.getNbLignes() )
    {
        std::cout << "Erreur : la matrice et le vecteur ne sont pas compatibles par produit" << std::endl;
    }
    else
    {
        res.setColonne( M.getEigenMatrice() * V.getColonne() );
    }
    return res;
}
Vecteur operator* ( Vecteur V, Matrice M )
{
    unsigned int n = V.getTaille();
    Vecteur res = Vecteur ( n );
    if ( n != M.getNbColonnes() )
    {
        std::cout << "Erreur : le vecteur et la matrice ne sont pas compatibles par produit" << std::endl;
    }
    else
    {
        res.setLigne( V.getLigne() * M.getEigenMatrice() );
    }
    return res;
}

