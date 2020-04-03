#include "mot.h"

// Les constructeurs
Mot::Mot ( )
{
    this->lettres = std::vector < lettre > ( );
}

// Les accesseurs
void Mot::push_back ( lettre l )
{
    this->lettres.push_back( l );
}

unsigned int Mot::size ( void ) const
{
    return this->lettres.size();
}

lettre Mot::at ( unsigned int i ) const
{
    return this->lettres.at(i);
}

QString Mot::toString() const
{
    QString str = QString();
    for ( unsigned int i = 0 ; i < this->lettres.size() ; i++ )
    {
        switch ( this->lettres.at(i) )
        {
            case T0 : str += "T0"; break;
            case T1 : str += "T1"; break;
            case T2 : str += "T2"; break;
            case T3 : str += "T3"; break;
            case P0 : str += "P0"; break;
            case P1 : str += "P1"; break;
            case P2 : str += "P2"; break;
            case P3 : str += "P3"; break;
            default : str += "?";  break;
        }
    }
    return str;
}
