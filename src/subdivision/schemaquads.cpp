#include "schemaquads.h"

SchemaQuads::SchemaQuads() : Schema()
{
    // rien
}
QString SchemaQuads::getTCS ( void ) const
{
    return "quads";
}
Mot SchemaQuads::calculMot( unsigned int precision, double u, double v ) const
{
    Mot mot = Mot ( );

    // Sécurité pour que u et v respectent la profondeur max
    double l = pow ( 2.0, precision );
    u = round ( u * l ) / l;
    v = round ( v * l ) / l;

    // La boucle principale
    while ( true )
    {
        // On trouve T_i
        unsigned int ur = floor ( u + 0.5 );
        unsigned int vr = floor ( v + 0.5 );
        unsigned int i = 2*vr + ( ur + vr )%2;

        // Si on a atteint le point fixe
        if ( u == ur && v == vr )
        {
            switch ( i )
            {
                case 0 :
                    mot.push_back( P0 );
                break;
                case 1 :
                    mot.push_back( P1 );
                break;
                case 2 :
                    mot.push_back( P2 );
                break;
                case 3 :
                    mot.push_back( P3 );
                break;
                default :
                    mot.push_back( P0 );
                break;
            }
            return mot;
        }
        // Sinon
        else
        {
            switch ( i )
            {
                case 0 :
                    mot.push_back( T0 );
                break;
                case 1 :
                    mot.push_back( T1 );
                break;
                case 2 :
                    mot.push_back( T2 );
                break;
                case 3 :
                    mot.push_back( T3 );
                break;
                default :
                    mot.push_back( T0 );
                break;
            }
            u = 2 * ( u - 0.5*ur );
            v = 2 * ( v - 0.5*vr );
        }
    }

    return mot; // Pas utile mais ça évite que le compilateur gueule
}

void SchemaQuads::genererMatrices( unsigned int valence_min, unsigned int valence_max )
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

void SchemaQuads::genererDictionnaire ( void )
{
    for ( double u = 0.0 ; u <= 1.0 ; u += 1.0 / 32 )
    {
        for ( double v = 0.0 ; v <= 1.0 ; v += 1.0 / 32 )
        {
            this->dictionnaire.push_back( this->calculMot( 6, u, v ) );
        }
    }
}

void SchemaQuads::calculCombinaison( unsigned int valence, Mot mot  )
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
