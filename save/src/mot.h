#ifndef MOT_H
#define MOT_H

#include <vector>
#include <cmath>
#include <QString>
#include <algorithm>

enum lettre { T0, T1, T2, T3, P0, P1, P2, P3, nb_lettres };

class Mot
{
    private :
        std::vector < lettre > lettres;

    public :
        Mot ( void );
        void push_back ( lettre l );
        unsigned int size ( void ) const;
        lettre at ( unsigned int i ) const;
        QString toString ( void ) const;
};

#endif // MOT_H
