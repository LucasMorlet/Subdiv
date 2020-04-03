#include "doosabin.h"

DooSabin::DooSabin( void )
{
    this->valence_ordinaire = 4;
    this->nom = QString ( "Doo-Sabin" );
}
QString DooSabin::getTES( void ) const
{
    return "doo_sabin";
}

void tri_patch ( std::vector < GLuint >& faces_adjacentes, std::vector < GLuint >& indices_faces )
{
    std::vector < GLuint > sommets_adjacents = std::vector < GLuint > ();
    // Pour chaque face, on prend les deux sommets qui encadrent le centre
    for ( unsigned int i = 0 ; i < faces_adjacentes.size() ; i++ )
    {
        sommets_adjacents.push_back( indices_faces.at( 4 * (faces_adjacentes[i] / 4) + ( faces_adjacentes[i] + 1 ) % 4 ) );
        sommets_adjacents.push_back( indices_faces.at( 4 * (faces_adjacentes[i] / 4) + ( faces_adjacentes[i] + 3 ) % 4 ) );
    }

    std::vector < GLuint > nouvelles_faces = std::vector < GLuint > ();
    nouvelles_faces.push_back( faces_adjacentes[0] );
    GLuint sommet_courant = sommets_adjacents[1];

    while ( nouvelles_faces.size() < faces_adjacentes.size() )
    {
        for ( unsigned int i = 2 ; i < sommets_adjacents.size() ; i += 2 )
        {
            if ( sommets_adjacents[i] == sommet_courant )
            {
                sommet_courant = sommets_adjacents[i+1];
                nouvelles_faces.push_back( faces_adjacentes[i/2] );
                break;
            }
        }
    }

    faces_adjacentes = nouvelles_faces;
}

void DooSabin::faces2patches( Objet& obj ) const
{
    std::vector < GLfloat > lesSommets  = obj.getSommets();
    std::vector < GLuint  > indices     = obj.getIndices();

    // On fait un coup de vrai Doo-Sabin pour avoir un maillage qui respecte les conditions de la méthode => 4 nouveaux sommets / face
    std::vector < GLfloat > nouveaux_sommets = std::vector < GLfloat > ();
    for ( unsigned int i = 0 ; i < indices.size() ; i += 4 )
    {
        GLuint a = indices[i];
        GLuint b = indices[i+1];
        GLuint c = indices[i+2];
        GLuint d = indices[i+3];

        GLfloat ax = lesSommets[3*a];
        GLfloat ay = lesSommets[3*a+1];
        GLfloat az = lesSommets[3*a+2];

        GLfloat bx = lesSommets[3*b];
        GLfloat by = lesSommets[3*b+1];
        GLfloat bz = lesSommets[3*b+2];

        GLfloat cx = lesSommets[3*c];
        GLfloat cy = lesSommets[3*c+1];
        GLfloat cz = lesSommets[3*c+2];

        GLfloat dx = lesSommets[3*d];
        GLfloat dy = lesSommets[3*d+1];
        GLfloat dz = lesSommets[3*d+2];

        nouveaux_sommets.push_back( ( 9 * ax + 3 * bx + 1 * cx + 3 * dx ) / 16.0 );
        nouveaux_sommets.push_back( ( 9 * ay + 3 * by + 1 * cy + 3 * dy ) / 16.0 );
        nouveaux_sommets.push_back( ( 9 * az + 3 * bz + 1 * cz + 3 * dz ) / 16.0 );

        nouveaux_sommets.push_back( ( 3 * ax + 9 * bx + 3 * cx + 1 * dx ) / 16.0 );
        nouveaux_sommets.push_back( ( 3 * ay + 9 * by + 3 * cy + 1 * dy ) / 16.0 );
        nouveaux_sommets.push_back( ( 3 * az + 9 * bz + 3 * cz + 1 * dz ) / 16.0 );

        nouveaux_sommets.push_back( ( 1 * ax + 3 * bx + 9 * cx + 3 * dx ) / 16.0 );
        nouveaux_sommets.push_back( ( 1 * ay + 3 * by + 9 * cy + 3 * dy ) / 16.0 );
        nouveaux_sommets.push_back( ( 1 * az + 3 * bz + 9 * cz + 3 * dz ) / 16.0 );

        nouveaux_sommets.push_back( ( 3 * ax + 1 * bx + 3 * cx + 9 * dx ) / 16.0 );
        nouveaux_sommets.push_back( ( 3 * ay + 1 * by + 3 * cy + 9 * dy ) / 16.0 );
        nouveaux_sommets.push_back( ( 3 * az + 1 * bz + 3 * cz + 9 * dz ) / 16.0 );
    }

    // On crée les patchs pour la méthode
    std::vector < GLuint > new_indices = std::vector < GLuint > ();
    std::vector < GLuint > les_patchs  = std::vector < GLuint > ();

    // Pour chaque sommet on trouve la liste des faces adjacentes
    // Attention une face adjacentes est le numéro d'apparition du sommet dans la liste des indices :
    // N° face = face / 4
    // Position du sommet central dans la face = face % 4
    for ( unsigned int i = 0 ; i < lesSommets.size() / 3 ; i++ )
    {
        std::vector < GLuint > faces_adjacentes = std::vector < GLuint > ();
        for ( unsigned int j = 0 ; j < indices.size() ; j++ )
        {
            if ( indices[j] == i )
            {
                faces_adjacentes.push_back ( j );
            }
        }
        if ( faces_adjacentes.size() >= 3 ) // On ne crée pas de patch sur les bords
        {
            tri_patch ( faces_adjacentes, indices );
            GLuint face_courante;
            // On crée un patch par face adjacente au sommet
            for ( unsigned int j = 0 ; j < faces_adjacentes.size() ; j++ )
            {
                les_patchs.push_back( new_indices.size() );
                // Sommet 0 (face courante)
                new_indices.push_back( faces_adjacentes[j] );
                // Sommets 1 et 2 (face précédente)
                face_courante = faces_adjacentes[ (j+faces_adjacentes.size()-1) % faces_adjacentes.size() ];
                new_indices.push_back( face_courante );
                new_indices.push_back( 4 * (face_courante / 4) + ( face_courante + 3 ) % 4 );
                // Sommets 3, 4, et 5 (face courante)
                face_courante = faces_adjacentes[j];
                new_indices.push_back( 4 * (face_courante / 4) + ( face_courante + 1 ) % 4 );
                new_indices.push_back( 4 * (face_courante / 4) + ( face_courante + 2 ) % 4 );
                new_indices.push_back( 4 * (face_courante / 4) + ( face_courante + 3 ) % 4 );
                // Sommets 6 et 7 (face suivante)
                face_courante = faces_adjacentes[ (j+1) % faces_adjacentes.size() ];
                new_indices.push_back( 4 * (face_courante / 4) + ( face_courante + 1 ) % 4 );
                new_indices.push_back( face_courante );
                // Sommets 8 à k+5 (faces supplémentaires)
                for ( unsigned int k = 2 ; k < faces_adjacentes.size()-1 ; k++ )
                {
                    face_courante = faces_adjacentes[ (j+k) % faces_adjacentes.size() ];
                    new_indices.push_back( face_courante );
                }
                les_patchs.push_back( new_indices.size() - les_patchs[ les_patchs.size() - 1 ] );
            }
        }
    }

    // On modifie l'objet
    obj.setSommetsPatchs( nouveaux_sommets );
    obj.setIndicesPatchs( new_indices );
    obj.setPatchs( les_patchs );
    obj.setNbPatchs( les_patchs.size() / 2 );
    obj.setFacesPatchs( new_indices );
}

// Les matrices
Matrice DooSabin::T_0( unsigned int k ) const
{
    double V = 9.0 / 16.0;
    double E = 3.0 / 16.0;
    double F = 1.0 / 16.0;
    std::vector < double > coeff = std::vector < double > ();
    coeff.push_back( ( k + 5.0 ) / ( 4.0 * k ) ); // Sommet central
    for ( unsigned int i = 1 ; i < k ; i++ )
    {
        coeff.push_back( ( 3.0 + 2.0 * cos ( 2*3.14159265*i / k ) ) / ( 4.0 * k ) );
    }
    Matrice M =  Matrice ( k + 5, k + 5 );

    // Ligne 0
    M.setCoeff( 0, 0, coeff[0] ) ;
    M.setCoeff( 0, 1, coeff[1] ) ;
    for ( unsigned int i = 1 ; i < k-1 ; i++ )
    {
        M.setCoeff( 0, 6+i, coeff[i] ) ;
    }

    // Ligne 1
    M.setCoeff( 1, 1, coeff[0] ) ;
    M.setCoeff( 1, 0, coeff[1] ) ;
    for ( unsigned int i = 2 ; i < k ; i++ )
    {
        M.setCoeff( 1, 5+i, coeff[i] ) ;
    }

    // Ligne 2
    M.setCoeff( 2, 1, V );
    M.setCoeff( 2, 2, E );
    M.setCoeff( 2, 3, F );
    M.setCoeff( 2, 0, E );

    // Ligne 3
    M.setCoeff( 3, 0, V );
    M.setCoeff( 3, 1, E );
    M.setCoeff( 3, 2, F );
    M.setCoeff( 3, 3, E );

    // Ligne 4
    M.setCoeff( 4, 0, V );
    M.setCoeff( 4, 3, E );
    M.setCoeff( 4, 4, F );
    M.setCoeff( 4, 5, E );

    // Ligne 5
    M.setCoeff( 5, 0, V );
    M.setCoeff( 5, 5, E );
    M.setCoeff( 5, 6, F );
    M.setCoeff( 5, 7, E );

    // Ligne 6
    M.setCoeff( 6, 7, V );
    M.setCoeff( 6, 0, E );
    M.setCoeff( 6, 5, F );
    M.setCoeff( 6, 6, E );

    // Ligne 7
    M.setCoeff( 7, 0, coeff[1] ) ;
    M.setCoeff( 7, 1, coeff[2] ) ;
    for ( unsigned int i = 0 ; i < k-2 ; i++ )
    {
        M.setCoeff( 7, 7+i, coeff[i] ) ;
    }

    // Lignes 8 à k+5
    for ( unsigned j = 8 ; j < k+5 ; j++ )
    {
        M.setCoeff( j, 0, coeff[ 1 + j-7 ] ) ;
        M.setCoeff( j, 1, coeff[ 2 + j-7 ] ) ;
        for ( unsigned int i = 7 ; i < k+5 ; i++ )
        {
            float absij = (i < j) ? j-i : i-j;
            M.setCoeff( j, i, coeff[ absij ] ) ;
        }
    }

    return M;
}

Matrice DooSabin::T_1( unsigned int k ) const
{
    double V = 9.0 / 16.0;
    double E = 3.0 / 16.0;
    double F = 1.0 / 16.0;
    std::vector < double > coeff = std::vector < double > ();
    coeff.push_back( ( k + 5.0 ) / ( 4.0 * k ) ); // Sommet central
    for ( unsigned int i = 1 ; i < k ; i++ )
    {
        coeff.push_back( ( 3.0 + 2.0 * cos ( 2*3.14159265*i / k ) ) / ( 4.0 * k ) );
    }
    Matrice M =  Matrice ( 9, k + 5 );

    // Ligne 0
    M.setCoeff( 0, 0, V );
    M.setCoeff( 0, 1, E );
    M.setCoeff( 0, 2, F );
    M.setCoeff( 0, 3, E );

    // Ligne 1
    M.setCoeff( 1, 1, V );
    M.setCoeff( 1, 2, E );
    M.setCoeff( 1, 3, F );
    M.setCoeff( 1, 0, E );

    // Ligne 2
    M.setCoeff( 2, 2, V );
    M.setCoeff( 2, 3, E );
    M.setCoeff( 2, 0, F );
    M.setCoeff( 2, 1, E );

    // Ligne 3
    M.setCoeff( 3, 3, V );
    M.setCoeff( 3, 0, E );
    M.setCoeff( 3, 1, F );
    M.setCoeff( 3, 2, E );

    // Ligne 4
    M.setCoeff( 4, 3, V );
    M.setCoeff( 4, 4, E );
    M.setCoeff( 4, 5, F );
    M.setCoeff( 4, 0, E );

    // Ligne 5
    M.setCoeff( 5, 0, V );
    M.setCoeff( 5, 3, E );
    M.setCoeff( 5, 4, F );
    M.setCoeff( 5, 5, E );

    // Ligne 6
    M.setCoeff( 6, 0, V );
    M.setCoeff( 6, 5, E );
    M.setCoeff( 6, 6, F );
    M.setCoeff( 6, 7, E );

    // Ligne 7
    M.setCoeff( 7, 0, coeff[0] ) ;
    M.setCoeff( 7, 1, coeff[1] ) ;
    for ( unsigned int i = 1 ; i < k-1 ; i++ )
    {
        M.setCoeff( 7, 6+i, coeff[i] ) ;
    }

    // Ligne 8
    M.setCoeff( 8, 1, coeff[0] ) ;
    M.setCoeff( 8, 0, coeff[1] ) ;
    for ( unsigned int i = 2 ; i < k ; i++ )
    {
        M.setCoeff( 8, 5+i, coeff[i] ) ;
    }

    return M;
}

Matrice DooSabin::T_2( unsigned int k ) const
{
    double V = 9.0 / 16.0;
    double E = 3.0 / 16.0;
    double F = 1.0 / 16.0;
    std::vector < double > coeff = std::vector < double > ();
    coeff.push_back( ( k + 5.0 ) / ( 4.0 * k ) ); // Sommet central
    for ( unsigned int i = 1 ; i < k ; i++ )
    {
        coeff.push_back( ( 3.0 + 2.0 * cos ( 2*3.14159265*i / k ) ) / ( 4.0 * k ) );
    }
    Matrice M =  Matrice ( 9, k + 5 );

    // Ligne 0
    M.setCoeff( 0, 0, V );
    M.setCoeff( 0, 3, E );
    M.setCoeff( 0, 4, F );
    M.setCoeff( 0, 5, E );

    // Ligne 1
    M.setCoeff( 1, 0, V );
    M.setCoeff( 1, 1, E );
    M.setCoeff( 1, 2, F );
    M.setCoeff( 1, 3, E );

    // Ligne 2
    M.setCoeff( 2, 3, V );
    M.setCoeff( 2, 0, E );
    M.setCoeff( 2, 1, F );
    M.setCoeff( 2, 2, E );

    // Ligne 3
    M.setCoeff( 3, 3, V );
    M.setCoeff( 3, 4, E );
    M.setCoeff( 3, 5, F );
    M.setCoeff( 3, 0, E );

    // Ligne 4
    M.setCoeff( 4, 4, V );
    M.setCoeff( 4, 5, E );
    M.setCoeff( 4, 0, F );
    M.setCoeff( 4, 3, E );

    // Ligne 5
    M.setCoeff( 5, 5, V );
    M.setCoeff( 5, 0, E );
    M.setCoeff( 5, 3, F );
    M.setCoeff( 5, 4, E );

    // Ligne 6
    M.setCoeff( 6, 5, V );
    M.setCoeff( 6, 6, E );
    M.setCoeff( 6, 7, F );
    M.setCoeff( 6, 0, E );

    // Ligne 7
    M.setCoeff( 7, 0, V );
    M.setCoeff( 7, 5, E );
    M.setCoeff( 7, 6, F );
    M.setCoeff( 7, 7, E );

    // Ligne 8
    M.setCoeff( 8, 0, coeff[0] ) ;
    M.setCoeff( 8, 1, coeff[1] ) ;
    for ( unsigned int i = 1 ; i < k-1 ; i++ )
    {
        M.setCoeff( 8, 6+i, coeff[i] ) ;
    }

    return M;
}
Matrice DooSabin::T_3( unsigned int k ) const
{
    double V = 9.0 / 16.0;
    double E = 3.0 / 16.0;
    double F = 1.0 / 16.0;
    std::vector < double > coeff = std::vector < double > ();
    coeff.push_back( ( k + 5.0 ) / ( 4.0 * k ) ); // Sommet central
    for ( unsigned int i = 1 ; i < k ; i++ )
    {
        coeff.push_back( ( 3.0 + 2.0 * cos ( 2*3.14159265*i / k ) ) / ( 4.0 * k ) );
    }
    Matrice M =  Matrice ( 9, k + 5 );

    // Ligne 0
    M.setCoeff( 0, 0, V );
    M.setCoeff( 0, 5, E );
    M.setCoeff( 0, 6, F );
    M.setCoeff( 0, 7, E );

    // Ligne 1
    M.setCoeff( 1, 0, coeff[0] ) ;
    M.setCoeff( 1, 1, coeff[1] ) ;
    for ( unsigned int i = 1 ; i < k-1 ; i++ )
    {
        M.setCoeff( 1, 6+i, coeff[i] ) ;
    }

    // Ligne 2
    M.setCoeff( 2, 0, V );
    M.setCoeff( 2, 1, E );
    M.setCoeff( 2, 2, F );
    M.setCoeff( 2, 3, E );

    // Ligne 3
    M.setCoeff( 3, 0, V );
    M.setCoeff( 3, 3, E );
    M.setCoeff( 3, 4, F );
    M.setCoeff( 3, 5, E );

    // Ligne 4
    M.setCoeff( 4, 5, V );
    M.setCoeff( 4, 0, E );
    M.setCoeff( 4, 3, F );
    M.setCoeff( 4, 4, E );

    // Ligne 5
    M.setCoeff( 5, 5, V );
    M.setCoeff( 5, 6, E );
    M.setCoeff( 5, 7, F );
    M.setCoeff( 5, 0, E );

    // Ligne 6
    M.setCoeff( 6, 6, V );
    M.setCoeff( 6, 7, E );
    M.setCoeff( 6, 0, F );
    M.setCoeff( 6, 5, E );

    // Ligne 7
    M.setCoeff( 7, 7, V );
    M.setCoeff( 7, 0, E );
    M.setCoeff( 7, 5, F );
    M.setCoeff( 7, 6, E );

    // Ligne 8
    M.setCoeff( 8, 0, coeff[1] ) ;
    M.setCoeff( 8, 1, coeff[2] ) ;
    for ( unsigned int i = 0 ; i < k-2 ; i++ )
    {
        M.setCoeff( 8, 7+i, coeff[i] ) ;
    }

    return M;
}

/* Les matrices des points fixes */
Matrice DooSabin::P_0 ( unsigned int k ) const
{
    Matrice M = Matrice ( 1, k+5 );

    Matrice first = T_0 (k);
    Matrice tmp = T_0 (k);
    for ( unsigned int i = 0 ; i < 10 ; i++ )
    {
        tmp = tmp * tmp;
    }
    tmp = tmp * first;

    for ( unsigned int i = 0 ; i < k+5 ; i++ )
    {
        M.setCoeff( 0, i, tmp.getCoeff( 0, i ));
    }

    return M;
}
Matrice DooSabin::P_1( unsigned int k ) const
{
    Matrice M = Matrice ( 1, k+5 );

    Matrice first = T_1 (k);
    Matrice tmp = T_1 (4);
    for ( unsigned int i = 0 ; i < 10 ; i++ )
    {
        tmp = tmp * tmp;
    }
    tmp = tmp * first;

    for ( unsigned int i = 0 ; i < k+5 ; i++ )
    {
        M.setCoeff( 0, i, tmp.getCoeff( 0, i ));
    }

    return M;
}
Matrice DooSabin::P_2( unsigned int k ) const
{
    Matrice M = Matrice ( 1, k+5 );

    Matrice first = T_2 (k);
    Matrice tmp = T_2 (4);
    for ( unsigned int i = 0 ; i < 10 ; i++ )
    {
        tmp = tmp * tmp;
    }
    tmp = tmp * first;

    for ( unsigned int i = 0 ; i < k+5 ; i++ )
    {
        M.setCoeff( 0, i, tmp.getCoeff( 0, i ));
    }

    return M;
}
Matrice DooSabin::P_3( unsigned int k ) const
{
    Matrice M = Matrice ( 1, k+5 );

    Matrice first = T_3 (k);
    Matrice tmp = T_3(4);
    for ( unsigned int i = 0 ; i < 10 ; i++ )
    {
        tmp = tmp * tmp;
    }
    tmp = tmp * first;

    for ( unsigned int i = 0 ; i < k+5 ; i++ )
    {
        M.setCoeff( 0, i, tmp.getCoeff( 0, i ));
    }

    return M;
}

Matrice DooSabin::Identity( unsigned int k ) const
{
    return Matrice ( k+5 );
}
