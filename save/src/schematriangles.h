#ifndef SCHEMATRIANGLES_H
#define SCHEMATRIANGLES_H

#include "schema.h"

class SchemaTriangles : public Schema
{
    public :
        SchemaTriangles();       
        QString getTCS ( void ) const;
        void genererCombinaisons ( unsigned int valence_min, unsigned int valence_max );

    protected :
        Mot calculMot ( unsigned int precision, double u, double v ) const;
        void genererMatrices( unsigned int valence_min, unsigned int valence_max );
};

#endif // SCHEMATRIANGLES_H
