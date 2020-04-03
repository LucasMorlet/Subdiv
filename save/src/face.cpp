#include "face.h"
/*
// Constructeur/destructeur
Face::Face( void )
{
    this->nb_voisins = 0;
}
Face::Face( GLuint a, GLuint b, GLuint c )
{
    this->nb_voisins = 0;
    this->sommets.push_back( a );
    this->sommets.push_back( b );
    this->sommets.push_back( c );
    //this->voisins = std::vector<GLuint>();
}
Face::Face( GLuint a, GLuint b, GLuint c, GLuint d )
{
    this->nb_voisins = 0;
    this->sommets.push_back( a );
    this->sommets.push_back( b );
    this->sommets.push_back( c );
    this->sommets.push_back( d );
    this->voisins_directs = std::vector<GLuint>();
    this->voisins_directs.assign(8, 0);
}

// Accesseurs
GLuint Face::getSommet ( int i ) const
{
    return this->sommets[i];
}
unsigned int Face::getNbSommets ( void ) const
{
    return this->sommets.size();
}
void Face::addVoisinsCatmull ( Face& f, GLuint a )
{
    // Sommet commun -> +1 sommet
    int pos = -1;
    for ( unsigned int i = 0 ; i < 4 ; i++ )
    {
        if ( this->sommets[i] == a )
        {
            pos = i;
            break;
        }
    }
    if ( pos >= 0 )
    {
        for ( unsigned int i = 0 ; i < 4 ; i++ )
        {
            if ( f.getSommet(i) == a )
            {
                this->voisins_faces.push_back( f.getSommet( (i+1)%4 ) );
                this->voisins_faces.push_back( f.getSommet( (i+2)%4 ) );
                this->voisins_faces.push_back( f.getSommet( (i+3)%4 ) );
                this->nb_voisins++;
                break;
            }
        }
    }
}
void Face::addVoisinsCatmull ( Face& f, GLuint a, GLuint b )
{
    // Arête commune -> +2 sommets
    int posA = -1;
    int posB = -1;
    for ( unsigned int i = 0 ; i < 4 ; i++ )
    {
        if ( this->sommets[i] == a )
        {
            posA = i;
        }
        else if ( this->sommets[i] == b )
        {
            posB = i;
        }
    }
    if ( posA >= 0 && posB >= 0 )
    {
        int pos = ( posA < posB ) ? posA : posB;
        if ( pos == 0 && ( posA == 3 || posB == 3 ) ) pos = 3;
        for ( unsigned int i = 0 ; i < 4 ; i++ )
        {
            if ( f.getSommet(i) == this->sommets[pos] )
            {
                this->voisins_directs[pos*2] = f.getSommet( (i+1)%4 );
                this->voisins_directs[pos*2+1] = f.getSommet( (i+2)%4 );
                this->nb_voisins += 2;
                break;
            }
        }
    }
}
void Face::getPatch ( std::vector<GLuint>& vec ) const
{
    if ( this->nb_voisins == this->voisins.size() ) // Si le vecteur de voisin n'a pas de trou
    {
        vec.insert( vec.end(), this->sommets.begin(), this->sommets.end() );
        vec.insert( vec.end(), this->voisins.begin(), this->voisins.end() );
    }
}

// Static
void Face::calculAdjacence ( Face& a, Face& b )
{
    std::vector<GLuint> sommetsCommuns;
    for ( unsigned int i = 0 ; i < a.getNbSommets() ; i++ )
    {
        for ( unsigned int j = 0 ; j < b.getNbSommets() ; j++ )
        {
            if ( a.getSommet(i) == b.getSommet(j) )
            {
                sommetsCommuns.push_back( a.getSommet(i) );
            }
        }
    }
    if ( sommetsCommuns.size() == 1 )
    {
        a.addVoisinsCatmull( b, sommetsCommuns[0] );
        b.addVoisinsCatmull( a, sommetsCommuns[0] );
    }
    else if ( sommetsCommuns.size() == 2 )
    {
        a.addVoisinsCatmull( b, sommetsCommuns[0], sommetsCommuns[1] );
        b.addVoisinsCatmull( a, sommetsCommuns[0], sommetsCommuns[1] );
    }
}
*/
