#include "matrice.h"

// Constructeur
Matrice::Matrice ( unsigned int size )
{
    this->nb_lignes = size;
    this->nb_colonnes = size;
    //this->matrice_armadillo = arma::mat( size, size, arma::fill::eye );
    this->matrice_eigen = Eigen::MatrixXd::Identity( size, size );
    this->point_fixe = std::vector < float >();
    this->tangenteU  = std::vector < float >();
    this->tangenteV  = std::vector < float >();
}
Matrice::Matrice( unsigned int L, unsigned int C )
{
    this->nb_lignes = L;
    this->nb_colonnes = C;
    //this->matrice_armadillo = arma::mat( L, C, arma::fill::zeros );
    this->matrice_eigen = Eigen::MatrixXd::Zero( L, C );
    this->point_fixe = std::vector < float >();
    this->tangenteU  = std::vector < float >();
    this->tangenteV  = std::vector < float >();
}
Matrice::Matrice ( unsigned int L, unsigned int C, std::vector <float> coeff )
{
    this->nb_lignes = L;
    this->nb_colonnes = C;
    this->coeff = coeff;
}
/*
Matrice::Matrice( unsigned int L, unsigned int C, arma::mat mat_arma, Eigen::MatrixXd mat_eig )
{
    this->nb_lignes = L;
    this->nb_colonnes = C;
    //this->matrice_armadillo = mat_arma;
    //this->matrice_eigen = mat_eig;
    this->point_fixe = std::vector < float >();
    this->tangenteU  = std::vector < float >();
    this->tangenteV  = std::vector < float >();
}
*/

// Accesseurs
unsigned int Matrice::getNbLignes ( void )
{
    return this->nb_lignes;
}
unsigned int Matrice::getNbColonnes ( void )
{
    return this->nb_colonnes;
}
/*
arma::mat Matrice::getArmaMatrice ( void )
{
    return this->matrice_armadillo;
}
*/
Eigen::MatrixXd Matrice::getEigenMatrice ( void )
{
    return this->matrice_eigen;
}
void Matrice::setEigenMatrice( Eigen::MatrixXd matrice )
{
    this->matrice_eigen = matrice;
}
bool Matrice::isSquare( void ) const
{
    return ( this->nb_lignes == this->nb_colonnes );
}
double Matrice::getCoeff ( unsigned int i, unsigned int j ) const
{
    //return this->matrice_armadillo.at ( i, j );
    return this->coeff.at( i*this->nb_colonnes + j );
}
void Matrice::setCoeff ( unsigned int i, unsigned int j, double val )
{
    //this->matrice_armadillo( i, j ) = val;
    this->matrice_eigen( i, j ) = val;
    this->coeff.at( i*this->nb_colonnes + j ) = val;
}
Matrice Matrice::getPointFixe( void ) const
{
    if ( this->point_fixe.size() == 0 )
    {
        std::cout << "Erreur : point fixe non-précalculé" << std::endl;
        return Matrice( 0, 0 );
    }
    Matrice res = Matrice ( 1, this->point_fixe.size() );
    for ( unsigned int i = 0 ; i < this->point_fixe.size() ; i++ )
    {
        res.setCoeff( 0, i, this->point_fixe.at(i) );
    }
    return res;
}
Matrice Matrice::getTangenteU( void ) const
{
    if ( this->tangenteU.size() == 0 )
    {
        std::cout << "Erreur : tangente-u non-précalculée" << std::endl;
        return Matrice( 0, 0 );
    }
    Matrice res = Matrice ( 1, this->tangenteU.size() );
    for ( unsigned int i = 0 ; i < this->tangenteU.size() ; i++ )
    {
        res.setCoeff( 0, i, this->tangenteU.at(i) );
    }
    return res;
}
Matrice Matrice::getTangenteV( void ) const
{
    if ( this->tangenteV.size() == 0 )
    {
        std::cout << "Erreur : tangente-v non-précalculée" << std::endl;
        return Matrice( 0, 0 );
    }
    Matrice res = Matrice ( 1, this->tangenteV.size() );
    for ( unsigned int i = 0 ; i < this->tangenteV.size() ; i++ )
    {
        res.setCoeff( 0, i, this->tangenteV.at(i) );
    }
    return res;
}


// ToString
QString Matrice::toString ( bool with_holes ) const
{
    QString str = QString();
    for ( unsigned int i = 0 ; i < this->nb_lignes ; i++ )
    {
        for ( unsigned int j = 0 ; j < this->nb_colonnes ; j++ )
        {
            if ( with_holes && this->getCoeff( i, j ) <= 0.001 )
            {
                str += "        ";
            }
            else
            {
                str += QString::number( this->getCoeff( i, j ), 'f', 3 ) + "   ";
            }
            if ( j % 4 >= 3 )
            {
                str += "|   ";
            }
        }
        str += "\n";
        if ( i % 4 >= 3 )
        {
            str += "\n";
        }
    }
    return str;
}

// Méthodes
void Matrice::normalize ( void )
{
    for ( unsigned int i = 0 ; i < nb_lignes ; i++ )
    {
        double somme = 0.0;
        for ( unsigned int j = 0 ; j < nb_colonnes ; j++ )
        {
            somme += this->getCoeff( i, j );
        }
        for ( unsigned int j = 0 ; j < nb_colonnes ; j++ )
        {
            this->setCoeff( i, j, this->getCoeff( i, j ) / somme );
        }
    }
}
Matrice Matrice::inverse( void ) const
{
    if ( !( this->isSquare() ) )
    {
        std::cout << "Erreur, tentative d'inversion d'une matrice non-carrée" << std::endl;
        return Matrice ( 0, 0 );
    }
    Matrice m ( this->nb_lignes, this->nb_colonnes );
    m.matrice_eigen = this->matrice_eigen.inverse();
    return m;
}

void Matrice::extractLeftEigenVectors ( void )
{
    if ( !( this->isSquare() ) )
    {
        std::cout << "Erreur, tentative d'eigen-analysis sur une matrice non-carrée" << std::endl;
    }
    if ( this->point_fixe.size() > 0 )
    {
        return; // On ne refait pas le calcul
    }

    /* Version Armadillo
    arma::cx_vec valeurs_propres_complexes = arma::cx_vec ( this->nb_colonnes );
    arma::cx_mat vecteurs_propres = arma::cx_mat ( this->nb_lignes, this->nb_colonnes );
    arma::eig_gen ( valeurs_propres_complexes, vecteurs_propres, this->matrice_armadillo );
    arma::vec valeurs_propres = real ( valeurs_propres_complexes );
    arma::mat vecteurs_propres_gauches = real ( inv ( vecteurs_propres ) );
    // Fin version Armadillo */

    /* Version Eigen */
    Eigen::EigenSolver<Eigen::MatrixXd> es( this->matrice_eigen );
    Eigen::VectorXcd valeurs_propres_complexes  = es.eigenvalues();
    Eigen::VectorXd valeurs_propres  = valeurs_propres_complexes.real();
    Eigen::MatrixXcd vecteurs_propres_complexes = es.eigenvectors();
    Eigen::MatrixXd vecteurs_propres_gauches = vecteurs_propres_complexes.inverse().real();

    // On trouve les trois plus grandes valeurs propres et leur position
    unsigned int indice_pf = 0, indice_u = 0, indice_v = 0;
    float val_pf = 0, val_u = 0, val_v = 0, valeur_courante;
    for ( unsigned int i = 0 ; i < this->nb_colonnes ; i++ )
    {
        //valeur_courante = valeurs_propres.at(i);
        valeur_courante = valeurs_propres[i];

        if ( fabs( valeur_courante ) > fabs ( val_pf ) )
        {
            indice_v = indice_u;
            val_v = val_u;
            indice_u = indice_pf;
            val_u = val_pf;
            indice_pf = i;
            val_pf = valeur_courante;
        }
        else if ( fabs( valeur_courante ) > fabs ( val_u ) )
        {
            indice_v = indice_u;
            val_v = val_u;
            indice_u = i;
            val_u = valeur_courante;
        }
        else if ( fabs( valeur_courante ) > fabs ( val_v ) )
        {
            indice_v = i;
            val_v = valeur_courante;
        }
    }

    // On extrait les trois vecteurs propres gauches associés aux plus grandes valeurs propres et on les corrige
    val_pf /= fabs ( val_pf );
    val_u  /= fabs ( val_u  );
    val_v  /= fabs ( val_v  );
    float somme_pf = 0, norme_u = 0, norme_v = 0;
    for ( unsigned int i = 0 ; i < this->nb_colonnes ; i++ )
    {
        this->point_fixe.push_back ( val_pf * vecteurs_propres_gauches ( indice_pf, i ) );
        this->tangenteU.push_back  ( val_u  * vecteurs_propres_gauches ( indice_u,  i  ) );
        this->tangenteV.push_back  ( val_v  * vecteurs_propres_gauches ( indice_v,  i ) );
        somme_pf += val_pf * vecteurs_propres_gauches ( indice_pf, i );
        norme_u  += vecteurs_propres_gauches ( indice_u, i ) * vecteurs_propres_gauches ( indice_u, i );
        norme_v  += vecteurs_propres_gauches ( indice_v, i ) * vecteurs_propres_gauches ( indice_v, i );
    }

    // On les stocke dans les vecteurs
    for ( unsigned int i = 0 ; i < this->nb_colonnes ; i++ )
    {
        this->point_fixe.at(i) /= somme_pf;
        this->tangenteU.at(i)  /= norme_u;
        this->tangenteV.at(i)  /= norme_v;
        // Pour plus de lisibilité en debug-mode
        if ( fabs( this->point_fixe.at(i) ) < 0.0001 ) this->point_fixe.at(i) = 0;
        if ( fabs( this->tangenteU .at(i) ) < 0.0001 ) this->tangenteU .at(i) = 0;
        if ( fabs( this->tangenteV .at(i) ) < 0.0001 ) this->tangenteV .at(i) = 0;
        // Fin lisibilité
    }
}
