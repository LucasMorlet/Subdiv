#include "catmullclark.h"

CatmullClark::CatmullClark( void ) : SchemaQuads()
{
    this->valence_ordinaire = 4;
    this->nom = QString ( "Catmull-Clark" );
}
QString CatmullClark::getTES( void ) const
{
    return "catmull_clark";
}

void tri_tableau ( std::vector<GLuint>& tab, GLuint centre, GLuint oppose )
{
    std::vector<GLuint> tab2 = std::vector < GLuint >();
    std::vector<GLuint> tab_res = std::vector < GLuint >();
    // On pivote les faces
    for ( unsigned int i = 0 ; i < tab.size() ; i++ )
    {
        if ( tab[i] == centre )
        {
            if ( tab[ 4*(i/4) + ((i+2)%4) ] == oppose ) // Si la face courante est celle dont on calcule le patch
            {
                tab_res.push_back( tab[ 4*(i/4) + ((i+3)%4) ] );
            }
            else
            {
                tab2.push_back( tab[ 4*(i/4) + ((i+1)%4) ] );
                tab2.push_back( tab[ 4*(i/4) + ((i+2)%4) ] );
                tab2.push_back( tab[ 4*(i/4) + ((i+3)%4) ] );
            }
        }
    }

    while ( tab2.size() > 0 )
    {
        unsigned int j;
        for ( j = 0 ; j < tab2.size() ; j += 3 )
        {
            if ( tab2[j] == tab_res[ tab_res.size()-1 ] ) break;
        }
        if ( j < tab2.size() )
        {
            tab_res.push_back( tab2[j+1] );
            tab_res.push_back( tab2[j+2] );
            tab2.erase( tab2.begin()+j, tab2.begin()+j+3 );
        }
        else
        {
            std::cout << "Erreur lors de la creation d'un patch" << std::endl;
            return;
        }
    }
    tab_res.erase( tab_res.end()-2, tab_res.end() );
    tab_res.erase( tab_res.begin(), tab_res.begin()+2 );
    tab = tab_res;
}

void CatmullClark::faces2patches( Objet& obj ) const
{
    std::vector < GLuint > indices = obj.getIndices();
    std::vector < GLuint > new_indices = std::vector < GLuint >();
    std::vector < GLuint > indices_patchs = std::vector < GLuint >();
    std::vector < GLuint > les_patchs  = std::vector < GLuint >();
    for ( unsigned int i = 0 ; i < indices.size() ; i += 4 )
    {
        // On récupère les sommets de la face courante
        GLuint a = indices[i];
        GLuint b = indices[i+1];
        GLuint c = indices[i+2];
        GLuint d = indices[i+3];
        // On crée pour chacun un tableau de sommets adjacents
        std::vector < GLuint > indicesA = std::vector < GLuint >();
        std::vector < GLuint > indicesB = std::vector < GLuint >();
        std::vector < GLuint > indicesC = std::vector < GLuint >();
        std::vector < GLuint > indicesD = std::vector < GLuint >();
        // On remplit les tableaux
        for ( unsigned int j = 0 ; j < indices.size() ; j++ )
        {
            if ( a == indices[j] )
                for ( unsigned int k = 0 ; k < 4 ; k++ )
                {
                    indicesA.push_back( indices[4*(j/4) + k] );
                }
            else if ( b == indices[j] )
                for ( unsigned int k = 0 ; k < 4 ; k++ )
                {
                    indicesB.push_back( indices[4*(j/4) + k] );
                }
            else if ( c == indices[j] )
                for ( unsigned int k = 0 ; k < 4 ; k++ )
                {
                    indicesC.push_back( indices[4*(j/4) + k] );
                }
            else if ( d == indices[j] )
                for ( unsigned int k = 0 ; k < 4 ; k++ )
                {
                    indicesD.push_back( indices[4*(j/4) + k] );
                }
        }
        // On trie les tableaux (si le sommet à plus de 3 faces incidentes)
        if ( indicesA.size() >= 12 && indicesB.size() >= 12 && indicesC.size() >= 12 && indicesD.size() >= 12 )
        {
            // On trie les faces
            tri_tableau( indicesA, a, c );
            tri_tableau( indicesB, b, d );
            tri_tableau( indicesC, c, a );
            tri_tableau( indicesD, d, b );

            // On les fusionne en commençant par le sommet extraordinaire s'il existe (on le suppose unique !)
            if ( indicesB.size() != 3 )
            {
                std::vector < GLuint > tmp = indicesA;
                indicesA = indicesB;
                indicesB = indicesC;
                indicesC = indicesD;
                indicesD = tmp;
                GLuint tmp2 = a;
                a = b;
                b = c;
                c = d;
                d = tmp2;
            }
            else if ( indicesC.size() != 3 )
            {
                std::vector < GLuint > tmp = indicesA;
                indicesA = indicesC;
                indicesC = tmp;
                tmp = indicesB;
                indicesB = indicesD;
                indicesD = tmp;
                GLuint tmp2 = a;
                a = c;
                c = tmp2;
                tmp2 = b;
                b = d;
                d = tmp2;
            }
            else if ( indicesD.size() != 3 )
            {
                std::vector < GLuint > tmp = indicesA;
                indicesA = indicesD;
                indicesD = indicesC;
                indicesC = indicesB;
                indicesB = tmp;
                GLuint tmp2 = a;
                a = d;
                d = c;
                c = b;
                b = tmp2;
            }

            // On ajoute le nouveau patch
            les_patchs.push_back( indices_patchs.size() );
            indices_patchs.push_back(a);
            indices_patchs.push_back(b);
            indices_patchs.push_back(c);
            indices_patchs.push_back(d);
            indices_patchs.push_back( indicesA.back() );
            indices_patchs.insert( indices_patchs.end(), indicesB.begin(), indicesB.end() );
            indices_patchs.insert( indices_patchs.end(), indicesC.begin(), indicesC.end() );
            indices_patchs.insert( indices_patchs.end(), indicesD.begin(), indicesD.end() );
            if ( indicesA.size() >= 3 )
            {
                indices_patchs.insert( indices_patchs.end(), indicesA.begin(), indicesA.end()-1 );
            }
            les_patchs.push_back( indices_patchs.size() - les_patchs.at(les_patchs.size()-1 ) );
            new_indices.push_back(a);
            new_indices.push_back(b);
            new_indices.push_back(c);
            new_indices.push_back(d);
        }
    }
    // On modifie l'objet
    obj.setIndicesPatchs( indices_patchs );
    obj.setFacesPatchs( new_indices );
    obj.setPatchs( les_patchs );
    obj.setNbPatchs( les_patchs.size() / 2 );
    obj.setSommetsPatchs( obj.getSommets() );
}

// Les matrices
Matrice CatmullClark::T_0( unsigned int k ) const
{
    double A = 1.0 / 4.0;
    double B = 3.0 / 8.0;
    double C = 1.0 / 16.0;
    double V = 9.0 / 16.0;
    double E = 6.0 / 64.0;
    double F = 1.0 / 64.0;
    double v = ( 4.0*k*k - 7.0*k ) / ( 4.0*k*k );
    double e = ( 6.0 ) / ( 4.0*k*k );
    double f = ( 1.0 ) / ( 4.0*k*k );
    Matrice M =  Matrice ( 2*k + 8, 2*k + 8 );

    // Ligne 0
    M.setCoeff ( 0, 0, v );
    M.setCoeff ( 0, 1, e );
    M.setCoeff ( 0, 2, f );
    M.setCoeff ( 0, 3, e );
    M.setCoeff ( 0, 4, e );
    M.setCoeff ( 0, 5, f );
    M.setCoeff ( 0, 13, f );
    for ( unsigned int i = 14 ; i < 2*k+8 ; i += 2 )
    {
        M.setCoeff ( 0, i, e );
        M.setCoeff ( 0, i+1, f );
    }

    // Ligne 1
    M.setCoeff ( 1, 0, B );
    M.setCoeff ( 1, 1, B );
    M.setCoeff ( 1, 2, C );
    M.setCoeff ( 1, 3, C );
    M.setCoeff ( 1, 4, C );
    M.setCoeff ( 1, 5, C );

    // Ligne 2
    M.setCoeff ( 2, 0, A );
    M.setCoeff ( 2, 1, A );
    M.setCoeff ( 2, 2, A );
    M.setCoeff ( 2, 3, A );

    // Ligne 3
    M.setCoeff ( 3, 0, B );
    M.setCoeff ( 3, 1, C );
    M.setCoeff ( 3, 2, C );
    M.setCoeff ( 3, 3, B );
    M.setCoeff ( 3, 13, C );
    if ( k >= 4 )
    {
        M.setCoeff ( 3, 14, C );
    }
    else
    {
        M.setCoeff ( 3, 4, C );
    }

    // Ligne 4
    M.setCoeff ( 4, 0, B );
    M.setCoeff ( 4, 1, C );
    M.setCoeff ( 4, 4, B );
    M.setCoeff ( 4, 5, C );
    if ( k >= 4 )
    {
        M.setCoeff ( 4, 2*k+6, C );
        M.setCoeff ( 4, 2*k+7, C );
    }
    else
    {
        M.setCoeff ( 4,  3, C );
        M.setCoeff ( 4, 13, C );
    }

    // Ligne 5
    M.setCoeff ( 5, 0, A );
    M.setCoeff ( 5, 1, A );
    M.setCoeff ( 5, 4, A );
    M.setCoeff ( 5, 5, A );

    // Ligne 6
    M.setCoeff ( 6, 0, C );
    M.setCoeff ( 6, 1, B );
    M.setCoeff ( 6, 4, C );
    M.setCoeff ( 6, 5, B );
    M.setCoeff ( 6, 6, C );
    M.setCoeff ( 6, 7, C );

    // Ligne 7
    M.setCoeff ( 7, 0, E );
    M.setCoeff ( 7, 1, V );
    M.setCoeff ( 7, 2, E );
    M.setCoeff ( 7, 3, F );
    M.setCoeff ( 7, 4, F );
    M.setCoeff ( 7, 5, E );
    M.setCoeff ( 7, 6, F );
    M.setCoeff ( 7, 7, E );
    M.setCoeff ( 7, 8, F );

    // Ligne 8
    M.setCoeff ( 8, 0, C );
    M.setCoeff ( 8, 1, B );
    M.setCoeff ( 8, 2, B );
    M.setCoeff ( 8, 3, C );
    M.setCoeff ( 8, 7, C );
    M.setCoeff ( 8, 8, C );

    // Ligne 9
    M.setCoeff ( 9,  0, F );
    M.setCoeff ( 9,  1, E );
    M.setCoeff ( 9,  2, V );
    M.setCoeff ( 9,  3, E );
    M.setCoeff ( 9,  7, F );
    M.setCoeff ( 9,  8, E );
    M.setCoeff ( 9,  9, F );
    M.setCoeff ( 9, 10, E );
    M.setCoeff ( 9, 11, F );

    // Ligne 10
    M.setCoeff ( 10,  0, C );
    M.setCoeff ( 10,  1, C );
    M.setCoeff ( 10,  2, B );
    M.setCoeff ( 10,  3, B );
    M.setCoeff ( 10, 10, C );
    M.setCoeff ( 10, 11, C );

    // Ligne 11
    M.setCoeff ( 11,  0, E );
    M.setCoeff ( 11,  1, F );
    M.setCoeff ( 11,  2, E );
    M.setCoeff ( 11,  3, V );
    M.setCoeff ( 11, 10, F );
    M.setCoeff ( 11, 11, E );
    M.setCoeff ( 11, 12, F );
    M.setCoeff ( 11, 13, E );
    if ( k >= 4 )
    {
        M.setCoeff ( 11, 14, F );
    }
    else
    {
        M.setCoeff ( 11, 4, F );
    }

    // Ligne 12
    M.setCoeff ( 12,  0, C );
    M.setCoeff ( 12,  3, B );
    M.setCoeff ( 12, 11, C );
    M.setCoeff ( 12, 12, C );
    M.setCoeff ( 12, 13, B );
    if ( k >= 4 )
    {
        M.setCoeff ( 12, 14, C );
    }
    else
    {
        M.setCoeff ( 12, 4, C );
    }

    // Ligne 13
    M.setCoeff ( 13,  0, A );
    M.setCoeff ( 13,  3, A );
    M.setCoeff ( 13, 13, A );
    if ( k >= 4 )
    {
        M.setCoeff ( 13, 14, A );
    }
    else
    {
        M.setCoeff ( 13, 4, A );
    }

    if ( k >= 4 )
    {
        // Ligne 14
        M.setCoeff ( 14,  0, B );
        M.setCoeff ( 14,  3, C );
        M.setCoeff ( 14, 13, C );
        M.setCoeff ( 14, 14, B );
        M.setCoeff ( 14, 15, C );
        if ( k > 4 )
        {
            M.setCoeff ( 14, 16, C );
        }
        else
        {
            M.setCoeff ( 14, 4, C );
        }

        // Ligne 15
        M.setCoeff ( 15,  0, A );
        M.setCoeff ( 15, 14, A );
        M.setCoeff ( 15, 15, A );
        if ( k > 4 )
        {
            M.setCoeff ( 15, 16, A );
        }
        else
        {
            M.setCoeff ( 15, 4, A );
        }

        // Les lignes 16 à 2k+7
        for ( unsigned int i = 16 ; i < 2*k+6 ; i += 2 )
        {
            M.setCoeff ( i, 0, B );
            M.setCoeff ( i, i-2, C );
            M.setCoeff ( i, i-1, C );
            M.setCoeff ( i, i, B );
            M.setCoeff ( i, i+1, C );
            M.setCoeff ( i, i+2, C );

            M.setCoeff ( i+1, 0, A );
            M.setCoeff ( i+1, i, A );
            M.setCoeff ( i+1, i+1, A );
            M.setCoeff ( i+1, i+2, A );
        }

        if ( k > 4 )
        {
            // Les deux dernières lignes (celles ou on reboucle sur le sommet 5)
            M.setCoeff ( 2*k+6, 0, B );
            M.setCoeff ( 2*k+6, 2*k+4, C );
            M.setCoeff ( 2*k+6, 2*k+5, C );
            M.setCoeff ( 2*k+6, 2*k+6, B );
            M.setCoeff ( 2*k+6, 2*k+7, C );
            M.setCoeff ( 2*k+6, 4, C );

            M.setCoeff ( 2*k+7, 0, A );
            M.setCoeff ( 2*k+7, 2*k+6, A );
            M.setCoeff ( 2*k+7, 2*k+7, A );
            M.setCoeff ( 2*k+7, 4, A );
        }
    }

    return M;
}
Matrice CatmullClark::T_1( unsigned int k ) const
{
    double A = 1.0 / 4.0;
    double B = 3.0 / 8.0;
    double C = 1.0 / 16.0;
    double V = 9.0 / 16.0;
    double E = 6.0 / 64.0;
    double F = 1.0 / 64.0;
    double v = ( 4.0*k*k - 7.0*k ) / ( 4.0*k*k );
    double e = ( 6.0 ) / ( 4.0*k*k );
    double f = ( 1.0 ) / ( 4.0*k*k );
    Matrice M =  Matrice ( 16, 2*k + 8 );

    // Ligne 1
    M.setCoeff ( 0, 0, B );
    M.setCoeff ( 0, 1, B );
    M.setCoeff ( 0, 2, C );
    M.setCoeff ( 0, 3, C );
    M.setCoeff ( 0, 4, C );
    M.setCoeff ( 0, 5, C );

    // Ligne 2
    M.setCoeff ( 1, 1, V );
    M.setCoeff ( 1, 0, E );
    M.setCoeff ( 1, 2, E );
    M.setCoeff ( 1, 5, E );
    M.setCoeff ( 1, 7, E );
    M.setCoeff ( 1, 3, F );
    M.setCoeff ( 1, 4, F );
    M.setCoeff ( 1, 6, F );
    M.setCoeff ( 1, 8, F );

    // Ligne 3
    M.setCoeff ( 2, 1, B );
    M.setCoeff ( 2, 2, B );
    M.setCoeff ( 2, 0, C );
    M.setCoeff ( 2, 3, C );
    M.setCoeff ( 2, 7, C );
    M.setCoeff ( 2, 8, C );

    // Ligne 4
    M.setCoeff ( 3, 0, A );
    M.setCoeff ( 3, 1, A );
    M.setCoeff ( 3, 2, A );
    M.setCoeff ( 3, 3, A );

    // Ligne 5
    M.setCoeff ( 4, 0, A );
    M.setCoeff ( 4, 1, A );
    M.setCoeff ( 4, 4, A );
    M.setCoeff ( 4, 5, A );

    // Ligne 6
    M.setCoeff ( 5, 1, B );
    M.setCoeff ( 5, 5, B );
    M.setCoeff ( 5, 0, C );
    M.setCoeff ( 5, 4, C );
    M.setCoeff ( 5, 6, C );
    M.setCoeff ( 5, 7, C );

    // Ligne 7
    M.setCoeff ( 6, 1, A );
    M.setCoeff ( 6, 5, A );
    M.setCoeff ( 6, 6, A );
    M.setCoeff ( 6, 7, A );

    // Ligne 8
    M.setCoeff ( 7, 1, B );
    M.setCoeff ( 7, 7, B );
    M.setCoeff ( 7, 2, C );
    M.setCoeff ( 7, 5, C );
    M.setCoeff ( 7, 6, C );
    M.setCoeff ( 7, 8, C );

    // Ligne 9
    M.setCoeff ( 8, 1, A );
    M.setCoeff ( 8, 2, A );
    M.setCoeff ( 8, 7, A );
    M.setCoeff ( 8, 8, A );

    // Ligne 10
    M.setCoeff ( 9,  2, B );
    M.setCoeff ( 9,  8, B );
    M.setCoeff ( 9,  1, C );
    M.setCoeff ( 9,  7, C );
    M.setCoeff ( 9,  9, C );
    M.setCoeff ( 9, 10, C );

    // Ligne 11
    M.setCoeff ( 10,  2, V );
    M.setCoeff ( 10,  1, E );
    M.setCoeff ( 10,  3, E );
    M.setCoeff ( 10,  8, E );
    M.setCoeff ( 10, 10, E );
    M.setCoeff ( 10,  0, F );
    M.setCoeff ( 10,  7, F );
    M.setCoeff ( 10, 9, F );
    M.setCoeff ( 10, 11, F );

    // Ligne 12
    M.setCoeff ( 11,  2, B );
    M.setCoeff ( 11,  3, B );
    M.setCoeff ( 11,  0, C );
    M.setCoeff ( 11,  1, C );
    M.setCoeff ( 11, 10, C );
    M.setCoeff ( 11, 11, C );

    // Ligne 13
    M.setCoeff ( 12,  3, V );
    M.setCoeff ( 12,  0, E );
    M.setCoeff ( 12,  2, E );
    M.setCoeff ( 12, 11, E );
    M.setCoeff ( 12, 13, E );
    M.setCoeff ( 12,  1, F );
    M.setCoeff ( 12, 10, F );
    M.setCoeff ( 12, 12, F );
    if ( k >= 4 )
        M.setCoeff ( 12, 14, F );
    else
        M.setCoeff ( 12,  4, F );

    // Ligne 14
    M.setCoeff ( 13,  0, B );
    M.setCoeff ( 13,  3, B );
    M.setCoeff ( 13,  1, C );
    M.setCoeff ( 13,  2, C );
    M.setCoeff ( 13, 13, C );
    if ( k >= 4 )
        M.setCoeff ( 13, 14, C );
    else
        M.setCoeff ( 13,  4, C );

    // Ligne 15
    M.setCoeff ( 14,  0, v );
    M.setCoeff ( 14,  1, e );
    M.setCoeff ( 14,  3, e );
    M.setCoeff ( 14,  4, e );
    M.setCoeff ( 14,  2, f );
    M.setCoeff ( 14,  5, f );
    M.setCoeff ( 14, 13, f );
    for ( unsigned int i = 14 ; i < 2*k + 8 ; i+=2 )
    {
        M.setCoeff ( 14,   i, e );
        M.setCoeff ( 14, i+1, f );
    }

    // Ligne 16
    M.setCoeff ( 15, 0, B );
    M.setCoeff ( 15, 4, B );
    M.setCoeff ( 15, 1, C );
    M.setCoeff ( 15, 5, C );
    if ( k >= 4 )
    {
        M.setCoeff ( 15, 2*k+6, C );
        M.setCoeff ( 15, 2*k+7, C );
    }
    else
    {
        M.setCoeff ( 15,  3, C );
        M.setCoeff ( 15, 13, C );
    }

    return M;
}
Matrice CatmullClark::T_2( unsigned int k ) const
{
    double A = 1.0 / 4.0;
    double B = 3.0 / 8.0;
    double C = 1.0 / 16.0;
    double V = 9.0 / 16.0;
    double E = 6.0 / 64.0;
    double F = 1.0 / 64.0;
    double v = ( 4.0*k*k - 7.0*k ) / ( 4.0*k*k );
    double e = ( 6.0 ) / ( 4.0*k*k );
    double f = ( 1.0 ) / ( 4.0*k*k );
    Matrice M =  Matrice ( 16, 2*k + 8 );

    // Ligne 1
    M.setCoeff ( 0, 0, A );
    M.setCoeff ( 0, 1, A );
    M.setCoeff ( 0, 2, A );
    M.setCoeff ( 0, 3, A );

    // Ligne 2
    M.setCoeff ( 1, 1, B );
    M.setCoeff ( 1, 2, B );
    M.setCoeff ( 1, 0, C );
    M.setCoeff ( 1, 3, C );
    M.setCoeff ( 1, 7, C );
    M.setCoeff ( 1, 8, C );

    // Ligne 3
    M.setCoeff ( 2,  2, V );
    M.setCoeff ( 2,  1, E );
    M.setCoeff ( 2,  3, E );
    M.setCoeff ( 2,  8, E );
    M.setCoeff ( 2, 10, E );
    M.setCoeff ( 2,  0, F );
    M.setCoeff ( 2,  7, F );
    M.setCoeff ( 2,  9, F );
    M.setCoeff ( 2, 11, F );

    // Ligne 4
    M.setCoeff ( 3,  2, B );
    M.setCoeff ( 3,  3, B );
    M.setCoeff ( 3,  0, C );
    M.setCoeff ( 3,  1, C );
    M.setCoeff ( 3, 10, C );
    M.setCoeff ( 3, 11, C );

    // Ligne 5
    M.setCoeff ( 4, 0, B );
    M.setCoeff ( 4, 1, B );
    M.setCoeff ( 4, 2, C );
    M.setCoeff ( 4, 3, C );
    M.setCoeff ( 4, 4, C );
    M.setCoeff ( 4, 5, C );

    // Ligne 6
    M.setCoeff ( 5, 1, V );
    M.setCoeff ( 5, 0, E );
    M.setCoeff ( 5, 2, E );
    M.setCoeff ( 5, 5, E );
    M.setCoeff ( 5, 7, E );
    M.setCoeff ( 5, 3, F );
    M.setCoeff ( 5, 4, F );
    M.setCoeff ( 5, 6, F );
    M.setCoeff ( 5, 8, F );

    // Ligne 7
    M.setCoeff ( 6, 1, B );
    M.setCoeff ( 6, 7, B );
    M.setCoeff ( 6, 2, C );
    M.setCoeff ( 6, 5, C );
    M.setCoeff ( 6, 6, C );
    M.setCoeff ( 6, 8, C );

    // Ligne 8
    M.setCoeff ( 7, 1, A );
    M.setCoeff ( 7, 2, A );
    M.setCoeff ( 7, 7, A );
    M.setCoeff ( 7, 8, A );

    // Ligne 9
    M.setCoeff ( 8,  2, B );
    M.setCoeff ( 8,  8, B );
    M.setCoeff ( 8,  1, C );
    M.setCoeff ( 8,  7, C );
    M.setCoeff ( 8,  9, C );
    M.setCoeff ( 8, 10, C );

    // Ligne 10
    M.setCoeff ( 9,  2, A );
    M.setCoeff ( 9,  8, A );
    M.setCoeff ( 9,  9, A );
    M.setCoeff ( 9, 10, A );

    // Ligne 11
    M.setCoeff ( 10,  2, B );
    M.setCoeff ( 10, 10, B );
    M.setCoeff ( 10,  3, C );
    M.setCoeff ( 10,  8, C );
    M.setCoeff ( 10,  9, C );
    M.setCoeff ( 10, 11, C );

    // Ligne 12
    M.setCoeff ( 11,  2, A );
    M.setCoeff ( 11,  3, A );
    M.setCoeff ( 11, 10, A );
    M.setCoeff ( 11, 11, A );

    // Ligne 13
    M.setCoeff ( 12,  3, B );
    M.setCoeff ( 12, 11, B );
    M.setCoeff ( 12,  2, C );
    M.setCoeff ( 12, 10, C );
    M.setCoeff ( 12, 12, C );
    M.setCoeff ( 12, 13, C );

    // Ligne 14
    M.setCoeff ( 13,  3, V );
    M.setCoeff ( 13,  0, E );
    M.setCoeff ( 13,  2, E );
    M.setCoeff ( 13, 11, E );
    M.setCoeff ( 13, 13, E );
    M.setCoeff ( 13,  1, F );
    M.setCoeff ( 13, 10, F );
    M.setCoeff ( 13, 12, F );
    if ( k >= 4 )
        M.setCoeff ( 13, 14, F );
    else
        M.setCoeff ( 13,  4, F );

    // Ligne 15
    M.setCoeff ( 14,  0, B );
    M.setCoeff ( 14,  3, B );
    M.setCoeff ( 14,  1, C );
    M.setCoeff ( 14,  2, C );
    M.setCoeff ( 14, 13, C );
    if ( k >= 4 )
        M.setCoeff ( 14, 14, C );
    else
        M.setCoeff ( 14,  4, C );


    // Ligne 16
    M.setCoeff ( 15,  0, v );
    M.setCoeff ( 15,  1, e );
    M.setCoeff ( 15,  3, e );
    M.setCoeff ( 15,  4, e );
    M.setCoeff ( 15,  2, f );
    M.setCoeff ( 15,  5, f );
    M.setCoeff ( 15, 13, f );
    for ( unsigned int i = 14 ; i < 2*k + 8 ; i+=2 )
    {
        M.setCoeff ( 15,   i, e );
        M.setCoeff ( 15, i+1, f );
    }
    return M;
}
Matrice CatmullClark::T_3( unsigned int k ) const
{
    double A = 1.0 / 4.0;
    double B = 3.0 / 8.0;
    double C = 1.0 / 16.0;
    double V = 9.0 / 16.0;
    double E = 6.0 / 64.0;
    double F = 1.0 / 64.0;
    double v = ( 4.0*k*k - 7.0*k ) / ( 4.0*k*k );
    double e = ( 6.0 ) / ( 4.0*k*k );
    double f = ( 1.0 ) / ( 4.0*k*k );
    Matrice M =  Matrice ( 16, 2*k + 8 );

    // Ligne 1
    M.setCoeff ( 0, 0, B );
    M.setCoeff ( 0, 3, B );
    M.setCoeff ( 0, 1, C );
    M.setCoeff ( 0, 2, C );
    M.setCoeff ( 0, 13, C );
    if ( k >= 4 )
        M.setCoeff ( 0, 14, C );
    else
        M.setCoeff ( 0,  4, C );

    // Ligne 2
    M.setCoeff ( 1, 0, A );
    M.setCoeff ( 1, 1, A );
    M.setCoeff ( 1, 2, A );
    M.setCoeff ( 1, 3, A );

    // Ligne 3
    M.setCoeff ( 2,  2, B );
    M.setCoeff ( 2,  3, B );
    M.setCoeff ( 2,  0, C );
    M.setCoeff ( 2,  1, C );
    M.setCoeff ( 2, 10, C );
    M.setCoeff ( 2, 11, C );

    // Ligne 4
    M.setCoeff ( 3,  3, V );
    M.setCoeff ( 3,  0, E );
    M.setCoeff ( 3,  2, E );
    M.setCoeff ( 3, 11, E );
    M.setCoeff ( 3, 13, E );
    M.setCoeff ( 3,  1, F );
    M.setCoeff ( 3, 10, F );
    M.setCoeff ( 3, 12, F );
    if ( k >= 4 )
        M.setCoeff ( 3, 14, F );
    else
        M.setCoeff ( 3,  4, F );

    // Ligne 5
    M.setCoeff ( 4,  0, v );
    M.setCoeff ( 4,  1, e );
    M.setCoeff ( 4,  3, e );
    M.setCoeff ( 4,  4, e );
    M.setCoeff ( 4,  2, f );
    M.setCoeff ( 4,  5, f );
    M.setCoeff ( 4, 13, f );
    for ( unsigned int i = 14 ; i < 2*k+8 ; i += 2 )
    {
        M.setCoeff ( 4,   i, e );
        M.setCoeff ( 4, i+1, f );
    }

    // Ligne 6
    M.setCoeff ( 5, 0, B );
    M.setCoeff ( 5, 1, B );
    M.setCoeff ( 5, 2, C );
    M.setCoeff ( 5, 3, C );
    M.setCoeff ( 5, 4, C );
    M.setCoeff ( 5, 5, C );

    // Ligne 7
    M.setCoeff ( 6, 1, V );
    M.setCoeff ( 6, 0, E );
    M.setCoeff ( 6, 2, E );
    M.setCoeff ( 6, 5, E );
    M.setCoeff ( 6, 7, E );
    M.setCoeff ( 6, 3, F );
    M.setCoeff ( 6, 4, F );
    M.setCoeff ( 6, 6, F );
    M.setCoeff ( 6, 8, F );

    // Ligne 8
    M.setCoeff ( 7, 1, B );
    M.setCoeff ( 7, 2, B );
    M.setCoeff ( 7, 0, C );
    M.setCoeff ( 7, 3, C );
    M.setCoeff ( 7, 7, C );
    M.setCoeff ( 7, 8, C );

    // Ligne 9
    M.setCoeff ( 8,  2, V );
    M.setCoeff ( 8,  1, E );
    M.setCoeff ( 8,  3, E );
    M.setCoeff ( 8,  8, E );
    M.setCoeff ( 8, 10, E );
    M.setCoeff ( 8,  0, F );
    M.setCoeff ( 8,  7, F );
    M.setCoeff ( 8,  9, F );
    M.setCoeff ( 8, 11, F );

    // Ligne 10
    M.setCoeff ( 9,  2, B );
    M.setCoeff ( 9, 10, B );
    M.setCoeff ( 9,  3, C );
    M.setCoeff ( 9,  8, C );
    M.setCoeff ( 9,  9, C );
    M.setCoeff ( 9, 11, C );

    // Ligne 11
    M.setCoeff ( 10,  2, A );
    M.setCoeff ( 10,  3, A );
    M.setCoeff ( 10, 10, A );
    M.setCoeff ( 10, 11, A );

    // Ligne 12
    M.setCoeff ( 11,  3, B );
    M.setCoeff ( 11, 11, B );
    M.setCoeff ( 11,  2, C );
    M.setCoeff ( 11, 10, C );
    M.setCoeff ( 11, 12, C );
    M.setCoeff ( 11, 13, C );

    // Ligne 13
    M.setCoeff ( 12,  3, A );
    M.setCoeff ( 12, 11, A );
    M.setCoeff ( 12, 12, A );
    M.setCoeff ( 12, 13, A );

    // Ligne 14
    M.setCoeff ( 13,  3, B );
    M.setCoeff ( 13, 13, B );
    M.setCoeff ( 13,  0, C );
    M.setCoeff ( 13, 11, C );
    M.setCoeff ( 13, 12, C );
    if ( k >= 4 )
        M.setCoeff ( 13, 14, C );
    else
        M.setCoeff ( 13,  4, C );

    // Ligne 15
    M.setCoeff ( 14,  0, A );
    M.setCoeff ( 14,  3, A );
    M.setCoeff ( 14, 13, A );
    if ( k >= 4 )
        M.setCoeff ( 14, 14, A );
    else
        M.setCoeff ( 14,  4, A );

    // Ligne 16
    M.setCoeff ( 15, 0, B );
    M.setCoeff ( 15, 3, C );
    M.setCoeff ( 15, 13, C );
    if ( k > 4 )
    {
        M.setCoeff ( 15, 14, B );
        M.setCoeff ( 15, 15, C );
        M.setCoeff ( 15, 16, C );
    }
    else if ( k == 4 )
    {
        M.setCoeff ( 15, 14, B );
        M.setCoeff ( 15, 15, C );
        M.setCoeff ( 15,  4, C );
    }
    else
    {
        M.setCoeff ( 15, 4, B );
        M.setCoeff ( 15, 1, C );
        M.setCoeff ( 15, 5, C );
    }
    return M;
}

/* Les matrices des points fixes */
Matrice CatmullClark::P_0 ( unsigned int k ) const
{
    // TODO à prouver
    double kd = (double)k;
    double div = kd + 5.0;
    double V =   kd / div;
    double F = 1 / ( kd *kd + 5.0 * kd );
    double E = 4 * F;

    Matrice M = Matrice ( 1, 2*k+8 );
    M.setCoeff( 0, 0, V );
    M.setCoeff( 0, 1, E );
    M.setCoeff( 0, 2, F );
    M.setCoeff( 0, 3, E );
    M.setCoeff( 0, 4, E );
    M.setCoeff( 0, 5, F );
    M.setCoeff( 0, 13, F );
    for ( unsigned int i = 14 ; i < 2*k+8 ; i += 2 )
    {
        M.setCoeff( 0, i, E );
        M.setCoeff( 0, i+1, F );
    }
    return M;
}
Matrice CatmullClark::P_1( unsigned int k ) const
{
    double V = 4.0 /  9.0;
    double E = 1.0 /  9.0;
    double F = 1.0 / 36.0;
    Matrice M = Matrice ( 1, 2*k+8 );
    M.setCoeff( 0, 1, V );

    M.setCoeff( 0, 0, E );
    M.setCoeff( 0, 2, E );
    M.setCoeff( 0, 5, E );
    M.setCoeff( 0, 7, E );

    M.setCoeff( 0, 3, F );
    M.setCoeff( 0, 4, F );
    M.setCoeff( 0, 6, F );
    M.setCoeff( 0, 8, F );
    return M;
}
Matrice CatmullClark::P_2( unsigned int k ) const
{
    double V = 4.0 /  9.0;
    double E = 1.0 /  9.0;
    double F = 1.0 / 36.0;
    Matrice M = Matrice ( 1, 2*k+8 );
    M.setCoeff( 0, 2, V );

    M.setCoeff( 0, 1,  E );
    M.setCoeff( 0, 3,  E );
    M.setCoeff( 0, 10, E );
    M.setCoeff( 0, 8,  E );

    M.setCoeff( 0, 0,  F );
    M.setCoeff( 0, 11, F );
    M.setCoeff( 0, 9,  F );
    M.setCoeff( 0, 7,  F );
    return M;
}
Matrice CatmullClark::P_3( unsigned int k ) const
{
    double V = 4.0 /  9.0;
    double E = 1.0 /  9.0;
    double F = 1.0 / 36.0;
    Matrice M = Matrice ( 1, 2*k+8 );
    M.setCoeff( 0,  3, V );

    M.setCoeff( 0,  0, E );
    M.setCoeff( 0,  2, E );
    M.setCoeff( 0, 11, E );
    M.setCoeff( 0, 13, E );

    M.setCoeff( 0,  1, F );
    M.setCoeff( 0, 10, F );
    M.setCoeff( 0, 12, F );
    if ( k >= 4 )
        M.setCoeff ( 0, 14, F );
    else
        M.setCoeff ( 0,  4, F );
    return M;
}

Matrice CatmullClark::Identity( unsigned int k ) const
{
    return Matrice ( 2*k+8 );
}
