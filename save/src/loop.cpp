#include "loop.h"

Loop::Loop( void )
{
    this->valence_ordinaire = 6;
    this->nom = QString ( "Loop" );
}
QString Loop::getTCS ( void ) const
{
    return "triangles_16";
}
QString Loop::getTES ( void ) const
{
    return "loop";
}

void tri_tableau2 ( std::vector<GLuint>& tab, GLuint centre, GLuint first )
{
    // On récupère les deux sommets qui ne sont pas "centre" de chaque face
    std::vector<GLuint> voisins = std::vector<GLuint>();
    for ( unsigned int i = 0 ; i < tab.size() ; i++ )
    {
        if ( tab[i] == centre )
        {
            if ( i%3 == 0 )
            {
                voisins.push_back( tab[i+1] );
                voisins.push_back( tab[i+2] );
            }
            else if ( i%3 == 1 )
            {
                voisins.push_back( tab[i+1] );
                voisins.push_back( tab[i-1] );
            }
            else
            {
                voisins.push_back( tab[i-2] );
                voisins.push_back( tab[i-1] );
            }
        }
    }
    // On met les sommets obtenus dans l'ordre trigonométrique
    tab.clear();
    while ( voisins.size() > 0 )
    {
        for ( unsigned int i = 0 ; i < voisins.size() ; i += 2 )
        {
            if ( voisins[i] == first )
            {
                tab.push_back( voisins[i+1] );
                first = voisins[i+1];
                voisins.erase( voisins.begin() + i, voisins.begin() + i + 2 );
                break;
            }
        }
    }
    // On enlève les deux derniers qui sont les sommets de la face courante
    tab.pop_back();
    tab.pop_back();
    // On supprime le premier indice qui est commun à deux tableaux
    tab.erase( tab.begin(), tab.begin() + 1 );
}

void Loop::faces2patches( Objet& obj ) const
{
    std::vector < GLuint > indices = obj.getIndices();
    std::vector < GLuint > new_indices = std::vector < GLuint >();
    std::vector < GLuint > indices_patchs = std::vector < GLuint >();
    std::vector < GLuint > les_patchs  = std::vector < GLuint >();
    for ( unsigned int i = 0 ; i < indices.size() ; i += 3 )
    {
        // On récupère les sommets de la face courante
        GLuint a = indices[i];
        GLuint b = indices[i+1];
        GLuint c = indices[i+2];
        // On crée pour chacun un tableau de sommets adjacents
        std::vector < GLuint > indicesA = std::vector < GLuint >();
        std::vector < GLuint > indicesB = std::vector < GLuint >();
        std::vector < GLuint > indicesC = std::vector < GLuint >();
        // On remplit les tableaux
        for ( unsigned int j = 0 ; j < indices.size() ; j++ )
        {
            if ( a == indices[j] )
                for ( unsigned int k = 0 ; k < 3 ; k++ )
                {
                    indicesA.push_back( indices[3*(j/3) + k] );
                }
            else if ( b == indices[j] )
                for ( unsigned int k = 0 ; k < 3 ; k++ )
                {
                    indicesB.push_back( indices[3*(j/3) + k] );
                }
            else if ( c == indices[j] )
                for ( unsigned int k = 0 ; k < 3 ; k++ )
                {
                    indicesC.push_back( indices[3*(j/3) + k] );
                }
        }
        //TODO On trie les tableaux (si le sommet à plus de 3 faces incidentes)
        if ( indicesA.size() >= 4*3 && indicesB.size() >= 4*3 && indicesC.size() >= 4*3 )
        {
            // On trie les faces
            tri_tableau2( indicesA, a, c );
            tri_tableau2( indicesB, b, a );
            tri_tableau2( indicesC, c, b );

            // On pivote de manière à ce que A soit le sommet extraordinaire (supposé unique)
            if ( indicesB.size() != 3 )
            {
                std::vector < GLuint > tmp = indicesA;
                indicesA = indicesB;
                indicesB = indicesC;
                indicesC = tmp;
                GLuint tmp2 = a;
                a = b;
                b = c;
                c = tmp2;
            }
            else if ( indicesC.size() != 3 )
            {
                std::vector < GLuint > tmp = indicesA;
                indicesA = indicesC;
                indicesC = indicesB;
                indicesB = tmp;
                GLuint tmp2 = a;
                a = c;
                c = b;
                b = tmp2;
            }

            // On sauvegarde la face pivotée
            new_indices.push_back(a);
            new_indices.push_back(b);
            new_indices.push_back(c);

            // On ajoute le nouveau patch
            les_patchs.push_back( indices_patchs.size() );

            indices_patchs.push_back(a);
            indices_patchs.push_back(b);
            indices_patchs.push_back(c);          
            indices_patchs.insert( indices_patchs.end(), indicesB.begin(), indicesB.end() );
            indices_patchs.insert( indices_patchs.end(), indicesC.begin(), indicesC.end() );
            indices_patchs.insert( indices_patchs.end(), indicesA.begin(), indicesA.end() );
            les_patchs.push_back( indices_patchs.size() - les_patchs.at(les_patchs.size()-1 ) );
        }
    }
    // On modifie l'objet
    obj.setIndicesPatchs( indices_patchs );
    obj.setPatchs( les_patchs );
    obj.setNbPatchs( les_patchs.size() / 2 );
    obj.setFacesPatchs( new_indices );
    obj.setSommetsPatchs( obj.getSommets() );
}


Matrice Loop::T_0 ( unsigned int k ) const
{
    double E = 3.0 / 8.0;
    double F = 1.0 / 8.0;

    double B = 3.0 / 48.0;
    double A = 1 - 6*B;

    double b = ( k <= 3 ) ? ( 3.0 / 16.0 ) : ( 3.0 / ( 8.0 * k ) );
    double a = 1 - k*b;

    Matrice M =  Matrice ( k + 6, k + 6 );

    // Ligne 0
    M.setCoeff( 0, 0, a );
    M.setCoeff( 0, 1, b );
    M.setCoeff( 0, 2, b );
    for ( unsigned int i = 8 ; i < k + 6 ; i++ )
    {
        M.setCoeff( 0, i, b );
    }

    // Ligne 1
    M.setCoeff( 1,   0, E );
    M.setCoeff( 1,   1, E );
    M.setCoeff( 1,   2, F );
    M.setCoeff( 1, k+5, F );

    // Ligne 2
    M.setCoeff( 2, 0, E );
    M.setCoeff( 2, 2, E );
    M.setCoeff( 2, 1, F );
    M.setCoeff( 2, 8, F );

    // Ligne 3
    M.setCoeff( 3,   1, E );
    M.setCoeff( 3, k+5, E );
    M.setCoeff( 3,   3, F );
    M.setCoeff( 3,   0, F );

    // Ligne 4
    M.setCoeff( 4,   1, A );
    M.setCoeff( 4,   3, B );
    M.setCoeff( 4,   4, B );
    M.setCoeff( 4,   5, B );
    M.setCoeff( 4,   2, B );
    M.setCoeff( 4,   0, B );
    M.setCoeff( 4, k+5, B );

    // Ligne 5
    M.setCoeff( 5, 1, E );
    M.setCoeff( 5, 2, E );
    M.setCoeff( 5, 0, F );
    M.setCoeff( 5, 5, F );

    // Ligne 6
    M.setCoeff( 6, 2, A );
    M.setCoeff( 6, 0, B );
    M.setCoeff( 6, 1, B );
    M.setCoeff( 6, 5, B );
    M.setCoeff( 6, 6, B );
    M.setCoeff( 6, 7, B );
    M.setCoeff( 6, 8, B );

    // Ligne 7
    M.setCoeff( 7, 2, E );
    M.setCoeff( 7, 8, E );
    M.setCoeff( 7, 0, F );
    M.setCoeff( 7, 7, F );

    if ( k == 3 )
    {
        // Ligne 8
        M.setCoeff( 8, 0, E );
        M.setCoeff( 8, 8, E );
        M.setCoeff( 8, 2, F );
        M.setCoeff( 8, 1, F );
    }
    else
    {
        // Ligne 8
        M.setCoeff( 8, 0, E );
        M.setCoeff( 8, 8, E );
        M.setCoeff( 8, 2, F );
        M.setCoeff( 8, 9, F );

        // Ligne 9 à k+4
        for ( unsigned int i = 9 ; i <= k+4 ; i++ )
        {
            M.setCoeff( i,   0, E );
            M.setCoeff( i,   i, E );
            M.setCoeff( i, i-1, F );
            M.setCoeff( i, i+1, F );
        }

        // Ligne k+5
        M.setCoeff( k+5,   0, E );
        M.setCoeff( k+5, k+5, E );
        M.setCoeff( k+5, k+4, F );
        M.setCoeff( k+5,   1, F );
    }

    return M;
}
Matrice Loop::T_1 ( unsigned int k ) const
{
    double E = 3.0 / 8.0;
    double F = 1.0 / 8.0;

    double B = 3.0 / 48.0;
    double A = 1 - 6*B;

    double b = ( k <= 3 ) ? ( 3.0 / 16.0 ) : ( 3.0 / ( 8.0 * k ) );
    double a = 1 - k*b;

    Matrice M =  Matrice ( 12, k + 6 );

    // Ligne 0
    M.setCoeff( 0,   0, E );
    M.setCoeff( 0,   1, E );
    M.setCoeff( 0,   2, F );
    M.setCoeff( 0, k+5, F );

    // Ligne 1
    M.setCoeff( 1,   1, A );
    M.setCoeff( 1,   3, B );
    M.setCoeff( 1,   4, B );
    M.setCoeff( 1,   5, B );
    M.setCoeff( 1,   2, B );
    M.setCoeff( 1,   0, B );
    M.setCoeff( 1, k+5, B );

    // Ligne 2
    M.setCoeff( 2, 1, E );
    M.setCoeff( 2, 2, E );
    M.setCoeff( 2, 0, F );
    M.setCoeff( 2, 5, F );

    // Ligne 3
    M.setCoeff( 3,   1, E );
    M.setCoeff( 3,   3, E );
    M.setCoeff( 3,   4, F );
    M.setCoeff( 3, k+5, F );

    // Ligne 4
    M.setCoeff( 4, 1, E );
    M.setCoeff( 4, 4, E );
    M.setCoeff( 4, 3, F );
    M.setCoeff( 4, 5, F );

    // Ligne 5
    M.setCoeff( 5, 1, E );
    M.setCoeff( 5, 5, E );
    M.setCoeff( 5, 4, F );
    M.setCoeff( 5, 2, F );

    // Ligne 6
    M.setCoeff( 6, 2, E );
    M.setCoeff( 6, 5, E );
    M.setCoeff( 6, 1, F );
    M.setCoeff( 6, 6, F );

    // Ligne 7
    M.setCoeff( 7, 2, A );
    M.setCoeff( 7, 0, B );
    M.setCoeff( 7, 1, B );
    M.setCoeff( 7, 5, B );
    M.setCoeff( 7, 6, B );
    M.setCoeff( 7, 7, B );
    M.setCoeff( 7, 8, B );

    // Ligne 8
    M.setCoeff( 8, 0, E );
    M.setCoeff( 8, 2, E );
    M.setCoeff( 8, 1, F );
    M.setCoeff( 8, 8, F );

    // Ligne 9
    M.setCoeff( 9, 0, a );
    M.setCoeff( 9, 1, b );
    M.setCoeff( 9, 2, b );
    for ( unsigned int i = 8 ; i < k + 6 ; i++ )
    {
        M.setCoeff( 9, i, b );
    }

    // Ligne 10
    M.setCoeff( 10,   0, E );
    M.setCoeff( 10, k+5, E );
    M.setCoeff( 10,   1, F );
    if ( k > 3 )
        M.setCoeff( 10, k+4, F );
    else
        M.setCoeff( 10,   2, F );

    // Ligne 11
    M.setCoeff( 11,   1, E );
    M.setCoeff( 11, k+5, E );
    M.setCoeff( 11,   0, F );
    M.setCoeff( 11,   3, F );

    return M;
}
Matrice Loop::T_2 ( unsigned int k ) const
{
    double E = 3.0 / 8.0;
    double F = 1.0 / 8.0;

    double B = 3.0 / 48.0;
    double A = 1 - 6*B;

    double b = ( k <= 3 ) ? ( 3.0 / 16.0 ) : ( 3.0 / ( 8.0 * k ) );
    double a = 1 - k*b;

    Matrice M =  Matrice ( 12, k + 6 );

    // Ligne 0
    M.setCoeff( 0, 0, E );
    M.setCoeff( 0, 2, E );
    M.setCoeff( 0, 1, F );
    M.setCoeff( 0, 8, F );

    // Ligne 1
    M.setCoeff( 1, 1, E );
    M.setCoeff( 1, 2, E );
    M.setCoeff( 1, 0, F );
    M.setCoeff( 1, 5, F );

    // Ligne 2
    M.setCoeff( 2, 2, A );
    M.setCoeff( 2, 0, B );
    M.setCoeff( 2, 1, B );
    M.setCoeff( 2, 5, B );
    M.setCoeff( 2, 6, B );
    M.setCoeff( 2, 7, B );
    M.setCoeff( 2, 8, B );

    // Ligne 3
    M.setCoeff( 3,   1, A );
    M.setCoeff( 3,   3, B );
    M.setCoeff( 3,   4, B );
    M.setCoeff( 3,   5, B );
    M.setCoeff( 3,   2, B );
    M.setCoeff( 3,   0, B );
    M.setCoeff( 3, k+5, B );

    // Ligne 4
    M.setCoeff( 4, 1, E );
    M.setCoeff( 4, 5, E );
    M.setCoeff( 4, 4, F );
    M.setCoeff( 4, 2, F );

    // Ligne 5
    M.setCoeff( 5, 2, E );
    M.setCoeff( 5, 5, E );
    M.setCoeff( 5, 1, F );
    M.setCoeff( 5, 6, F );

    // Ligne 6
    M.setCoeff( 6, 2, E );
    M.setCoeff( 6, 6, E );
    M.setCoeff( 6, 5, F );
    M.setCoeff( 6, 7, F );

    // Ligne 7
    M.setCoeff( 7, 2, E );
    M.setCoeff( 7, 7, E );
    M.setCoeff( 7, 6, F );
    M.setCoeff( 7, 8, F );

    // Ligne 8
    M.setCoeff( 8, 2, E );
    M.setCoeff( 8, 8, E );
    M.setCoeff( 8, 0, F );
    M.setCoeff( 8, 7, F );

    // Ligne 9
    M.setCoeff( 9, 0, E );
    M.setCoeff( 9, 8, E );
    M.setCoeff( 9, 2, F );
    if ( k == 3 )
    {
        M.setCoeff( 9, 1, F );
    }
    else
    {
        M.setCoeff( 9, 9, F );
    }

    // Ligne 10
    M.setCoeff( 10, 0, a );
    M.setCoeff( 10, 1, b );
    M.setCoeff( 10, 2, b );
    for ( unsigned int i = 8 ; i < k + 6 ; i++ )
    {
        M.setCoeff( 10, i, b );
    }

    // Ligne 11
    M.setCoeff( 11,   0, E );
    M.setCoeff( 11,   1, E );
    M.setCoeff( 11,   2, F );
    M.setCoeff( 11, k+5, F );

    return M;
}
Matrice Loop::T_3 ( unsigned int k ) const
{
    double E = 3.0 / 8.0;
    double F = 1.0 / 8.0;

    double B = 3.0 / 48.0;
    double A = 1 - 6*B;

    double b = ( k <= 3 ) ? ( 3.0 / 16.0 ) : ( 3.0 / ( 8.0 * k ) );
    double a = 1 - k*b;

    Matrice M =  Matrice ( 12, k + 6 );

    // Ligne 0
    M.setCoeff( 0,   0, E );
    M.setCoeff( 0,   1, E );
    M.setCoeff( 0,   2, F );
    M.setCoeff( 0, k+5, F );

    // Ligne 1
    M.setCoeff( 1, 1, E );
    M.setCoeff( 1, 2, E );
    M.setCoeff( 1, 0, F );
    M.setCoeff( 1, 5, F );

    // Ligne 2
    M.setCoeff( 2, 0, E );
    M.setCoeff( 2, 2, E );
    M.setCoeff( 2, 1, F );
    M.setCoeff( 2, 8, F );

    // Ligne 3
    M.setCoeff( 3, 1, E );
    M.setCoeff( 3, 5, E );
    M.setCoeff( 3, 4, F );
    M.setCoeff( 3, 2, F );

    // Ligne 4
    M.setCoeff( 4, 2, E );
    M.setCoeff( 4, 5, E );
    M.setCoeff( 4, 1, F );
    M.setCoeff( 4, 6, F );

    // Ligne 5
    M.setCoeff( 5, 2, A );
    M.setCoeff( 5, 0, B );
    M.setCoeff( 5, 1, B );
    M.setCoeff( 5, 5, B );
    M.setCoeff( 5, 6, B );
    M.setCoeff( 5, 7, B );
    M.setCoeff( 5, 8, B );

    // Ligne 6
    M.setCoeff( 6, 2, E );
    M.setCoeff( 6, 8, E );
    M.setCoeff( 6, 0, F );
    M.setCoeff( 6, 7, F );

    // Ligne 7
    M.setCoeff( 7, 0, E );
    M.setCoeff( 7, 8, E );
    M.setCoeff( 7, 2, F );
    if ( k == 3 )
    {
        M.setCoeff( 7, 1, F );
    }
    else
    {
        M.setCoeff( 7, 9, F );
    }

    // Ligne 8
    M.setCoeff( 8, 0, a );
    M.setCoeff( 8, 1, b );
    M.setCoeff( 8, 2, b );
    for ( unsigned int i = 8 ; i < k + 6 ; i++ )
    {
        M.setCoeff( 8, i, b );
    }

    // Ligne 9
    M.setCoeff( 9,   0, E );
    M.setCoeff( 9, k+5, E );
    M.setCoeff( 9,   1, F );
    if ( k > 3 )
        M.setCoeff( 9, k+4, F );
    else
        M.setCoeff( 9,   2, F );

    // Ligne 10
    M.setCoeff( 10,   1, E );
    M.setCoeff( 10, k+5, E );
    M.setCoeff( 10,   0, F );
    M.setCoeff( 10,   3, F );

    // Ligne 11
    M.setCoeff( 11,   1, A );
    M.setCoeff( 11,   3, B );
    M.setCoeff( 11,   4, B );
    M.setCoeff( 11,   5, B );
    M.setCoeff( 11,   2, B );
    M.setCoeff( 11,   0, B );
    M.setCoeff( 11, k+5, B );

    return M;
}
Matrice Loop::P_0 ( unsigned int k ) const
{
    Matrice M = this->T_0(k);
    M = M*M*M*M*M*M*M*M*M*M*M*M*M*M*M*M;

    Matrice vec = Matrice ( 1, k+6 );
    for ( unsigned int i = 0 ; i < k+6 ; i++ )
    {
        vec.setCoeff( 0, i, M.getCoeff(0, i) );
    }
    return vec;
}
Matrice Loop::P_1 ( unsigned int k ) const
{
    Matrice M = this->T_1(6);
    Matrice N = this->T_1(k);
    M = M*M*M*M*M*M*M*M*M*M*M*M*M*M*M*N;

    Matrice vec = Matrice ( 1, k+6 );
    for ( unsigned int i = 0 ; i < k+6 ; i++ )
    {
        vec.setCoeff( 0, i, M.getCoeff(0, i) );
    }
    return vec;
}
Matrice Loop::P_2 ( unsigned int k ) const
{
    Matrice M = this->T_2(6);
    Matrice N = this->T_2(k);
    M = M*M*M*M*M*M*M*M*M*M*M*M*M*M*M*N;

    Matrice vec = Matrice ( 1, k+6 );
    for ( unsigned int i = 0 ; i < k+6 ; i++ )
    {
        vec.setCoeff( 0, i, M.getCoeff(0, i) );
    }
    return vec;
}
Matrice Loop::P_3 ( unsigned int k ) const
{
    Matrice M = this->T_3(6);
    Matrice N = this->T_3(k);
    M = M*M*M*M*M*M*M*M*M*M*M*M*M*M*M*N;

    Matrice vec = Matrice ( 1, k+6 );
    for ( unsigned int i = 0 ; i < k+6 ; i++ )
    {
        vec.setCoeff( 0, i, M.getCoeff(0, i) );
    }
    return vec;
}
Matrice Loop::Identity( unsigned int k ) const
{
    return Matrice ( k + 6 );
}
