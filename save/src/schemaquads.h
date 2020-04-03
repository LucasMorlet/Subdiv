#ifndef SCHEMAQUADS_H
#define SCHEMAQUADS_H

#include "schema.h"

class SchemaQuads : public Schema
{
    public :
        SchemaQuads();        
        QString getTCS ( void ) const override;
        void genererCombinaisons ( unsigned int valence_min, unsigned int valence_max ) override;

    protected :
        Mot calculMot ( unsigned int precision, double u, double v ) const override;
        void genererMatrices( unsigned int valence_min, unsigned int valence_max ) override;
};

#endif // SCHEMAQUADS_H
