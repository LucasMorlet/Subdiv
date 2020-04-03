#include "matrice.h"

// Constructeur
Matrice::Matrice ( unsigned int size )
{
    this->nb_lignes = size;
    this->nb_colonnes = size;
    this->M = boost::numeric::ublas::identity_matrix < double > ( size );
}
Matrice::Matrice( unsigned int L, unsigned int C )
{
    this->nb_lignes = L;
    this->nb_colonnes = C;
    this->M = boost::numeric::ublas::matrix < double > ( L, C );
    this->M.clear();
}
Matrice::Matrice( unsigned int L, unsigned int C, boost::numeric::ublas::matrix < double > mat )
{
    this->nb_lignes = L;
    this->nb_colonnes = C;
    this->M = mat;
}

// Accesseurs
unsigned int Matrice::getNbLignes ( void )
{
    return this->nb_lignes;
}
unsigned int Matrice::getNbColonnes ( void )
{
    return this->nb_colonnes;
}
boost::numeric::ublas::matrix < double > Matrice::getMatrice ( void )
{
    return this->M;
}

// Accesseurs des coeeficients
double Matrice::getCoeff ( unsigned int i, unsigned int j ) // const
{
    return this->M.at_element( i, j );
}
void Matrice::setCoeff ( unsigned int i, unsigned int j, double val )
{
    this->M.at_element( i, j ) = val;
}
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

// Communication avec le shader
void Matrice::passToShader( ShaderProgram* progShad, const QString& str ) // const
{
    // Construction du vecteur à envoyer
    GLfloat tab[ this->nb_lignes * this->nb_colonnes ];
    for ( unsigned int i = 0 ; i < this->nb_lignes ; i++ )
    {
        for ( unsigned int j = 0 ; j < this->nb_colonnes ; j++ )
        {
            tab [ i * (this->nb_colonnes) + j] = this->getCoeff(i, j);
        }
    }
    // Passage au shader
    glUniform1fv ( glGetUniformLocation( progShad->getIndice(), str.toStdString().c_str() ), this->nb_lignes * this->nb_colonnes, tab );
}

// ToString
QString Matrice::toString ( bool with_holes )
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

// Surcharge d'opérateur
Matrice operator* ( Matrice A, Matrice B )
{
    if ( A.getNbColonnes() != B.getNbLignes() )
    {
        std::cout << "Erreur : les matrices ne sont pas compatibles par produit" << std::endl;
        return Matrice( 0, 0 );
    }
    else
    {
        return Matrice ( A.getNbLignes(), B.getNbColonnes(), prod ( A.getMatrice(), B.getMatrice() ) );
    }
}
