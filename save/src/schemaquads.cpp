#include "schemaquads.h"

SchemaQuads::SchemaQuads() : Schema()
{
    //this->liste_coeff = std::vector<float>();
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
void SchemaQuads::genererCombinaisons ( unsigned int valence_min, unsigned int valence_max )
{
    if ( this->liste_coeff.size() <= 0 )
    {
        // On génère toutes les matrices qui nous seront utiles
        this->genererMatrices( valence_min, valence_max );

        // On génère l'ensemble des mots
        std::vector < Mot > dictionnaire = std::vector < Mot > ();
        for ( double u = 0.0 ; u <= 1.0 ; u += 1.0 / 32 )
        {
            for ( double v = 0.0 ; v <= 1.0 ; v += 1.0 / 32 )
            {
                dictionnaire.push_back( this->calculMot( 6, u, v ) );
            }
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

        tmp_matrices.push_back( this->P_0(i) );
        tmp_matrices.push_back( this->P_1(i) );
        tmp_matrices.push_back( this->P_2(i) );
        tmp_matrices.push_back( this->P_3(i) );

        this->liste_matrices.push_back( tmp_matrices );
    }
}


