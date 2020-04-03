#include "schematriangles.h"

SchemaTriangles::SchemaTriangles()
{
    // rien
}
QString SchemaTriangles::getTCS ( void ) const
{
    return "triangles";
}
Mot SchemaTriangles::calculMot ( unsigned int precision, double u, double v ) const
{
    Mot mot = Mot ( );

    // Sécurité pour que u et v respectent la profondeur max
    double l = pow ( 2.0, precision );
    u = round ( u * l ) / l;
    v = round ( v * l ) / l;
    double w = 1.0 - u - v;

    // La boucle principale
    while ( true )
    {
        // Si on atteint un point fixe
        if ( u == 1.0 )
        {
            mot.push_back( P0 );
            return mot;
        }
        else if ( v == 1.0 )
        {
            mot.push_back( P1 );
            return mot;
        }
        else if ( w == 1.0 )
        {
            mot.push_back( P2 );
            return mot;
        }
        else if ( u == v && v == w ) // Impossible d'atteindre ( 1/3 ; 1/3 ; 1/3 ) en divisant par 2
        {
            mot.push_back( P3 );
            return mot;
        }
        // Sinon
        else
        {
            if ( u >= 0.5 )
            {
                mot.push_back( T0 );
                u = 2*u - 1;
                v = 2*v;
                w = 2*w;
            }
            else if ( v >= 0.5 )
            {
                mot.push_back( T1 );
                u = 2*u;
                v = 2*v - 1;
                w = 2*w;
            }
            else if ( w >= 0.5 )
            {
                mot.push_back( T2 );
                u = 2*u;
                v = 2*v;
                w = 2*w - 1;
            }
            else
            {
                mot.push_back( T3 );
                double U = u + v - w;
                double V = v + w - u;
                double W = u + w - v;
                u = U;
                v = V;
                w = W;
            }
        }
    }

    return mot; // Pas utile mais ça évite que le compilateur gueule
}

/*
void SchemaTriangles::genererCombinaisons ( unsigned int valence_min, unsigned int valence_max )
{   
    // On génère toutes les matrices qui nous seront utiles
    this->genererMatrices( valence_min, valence_max );
    for ( unsigned int i = T0 ; i <= T3 ; i++ )
    {
        this->liste_matrices.at( this->valence_ordinaire ).at(i).extractLeftEigenVectors();
    }
    for ( unsigned int i = valence_min ; i <= valence_max ; i++ )
    {
        this->liste_matrices.at( i ).at( T0 ).extractLeftEigenVectors();
    }    

    // On génère l'ensemble des combinaisons ( mots x valence )
    for ( unsigned int k = valence_min ; k <= valence_max ; k++ )
    {
        for ( unsigned int i = 0 ; i < dictionnaire.size() ; i++ )
        {
            this->calculCombinaison( k, dictionnaire.at(i) );
        }
    }
}
*/

void SchemaTriangles::genererDictionnaire ( void )
{
    for ( double w = 0.0 ; w <= 1.0 ; w += 0.25 )
    {
        for ( double u = 1.0 - w ; u >= 0.0 ; u -= 0.25 )
        {
            this->dictionnaire.push_back( this->calculMot( 3, u, 1-u-w ) );
        }
    }
}

void SchemaTriangles::genererMatrices( unsigned int valence_min, unsigned int valence_max )
{
    std::vector<Matrice> tmp_matrices = std::vector<Matrice>();
    // Le tampon de départ pour que la valence soit l'indice du tableau
    for ( unsigned int i = 0 ; i < valence_min ; i++ )
    {
        this->liste_matrices.push_back( tmp_matrices );
    }
    // La génération des matrices
    for ( unsigned int i = valence_min ; i <= valence_max ; i++ )
    {
        tmp_matrices.clear();

        tmp_matrices.push_back( this->T_0(i) );
        tmp_matrices.push_back( this->T_1(i) );
        tmp_matrices.push_back( this->T_2(i) );
        tmp_matrices.push_back( this->T_3(i) );
/*
        tmp_matrices.push_back( this->P_0(i) );
        tmp_matrices.push_back( this->P_1(i) );
        tmp_matrices.push_back( this->P_2(i) );
        tmp_matrices.push_back( this->P_3(i) );
*/
        this->liste_matrices.push_back( tmp_matrices );
    }

    // Le calcul des vecteurs propres intéressants
    for ( unsigned int i = T0 ; i <= T3 ; i++ )
    {
        this->liste_matrices.at( this->valence_ordinaire ).at(i).extractLeftEigenVectors();
    }
    for ( unsigned int i = valence_min ; i <= valence_max ; i++ )
    {
        this->liste_matrices.at( i ).at( T0 ).extractLeftEigenVectors();
    }
}

void SchemaTriangles::calculCombinaison( unsigned int valence, Mot mot  )
{
    // On load la matrice identité du schéma
    Matrice M = this->Identity( valence );
    // On applique toutes les transformations T/M
    for ( unsigned int i = 0 ; i < mot.size()-1 ; i++ )
    {
        M = this->getMatrice( mot.at(i), valence ) * M;
        if ( mot.at(i) != T0 )
        {
            valence = this->valence_ordinaire;
        }
    }
    // On applique la transformation point-fixe et les transformations de tangentes
    Matrice P = Matrice ( 0, 0 );
    switch ( mot.at( mot.size()-1 ) )
    {
        case P0 :
        {
            P = this->getMatrice( T0, valence );
        }
        break;
        case P1 :
        {
            M = this->getMatrice( T1, valence ) * M;
            P = this->getMatrice( T1, this->valence_ordinaire );
        }
        break;
        case P2 :
        {
            M = this->getMatrice( T2, valence ) * M;
            P = this->getMatrice( T2, this->valence_ordinaire );
        }
        break;
        case P3 :
        {
            M = this->getMatrice( T3, valence ) * M;
            P = this->getMatrice( T3, this->valence_ordinaire );
        }
        break;

        // Les cas qui n'arrivent jamais
        case T0 :
        case T1 :
        case T2 :
        case T3 :
        default :
        {
            return; // On arrête le massacre
        }
    }

    Matrice U = P.getTangenteU() * M;
    Matrice V = P.getTangenteV() * M;
    M         = P.getPointFixe() * M;

    for ( unsigned int i = 0 ; i < M.getNbColonnes() ; i++ )
    {
        this->liste_coeff.  push_back( M.getCoeff( 0, i ) );
        this->liste_coeff_u.push_back( U.getCoeff( 0, i ) );
        this->liste_coeff_v.push_back( V.getCoeff( 0, i ) );
    }
}
