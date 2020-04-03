#include "materiau.h"

Materiau::Materiau()
{
    this->coeff_ambiant    = 0.2f;
    this->coeff_speculaire = 0.7f;
    this->coeff_diffus     = 0.6f;
    this->hardness         = 8;
}

void Materiau::sendToShader( GLuint indiceShader ) const
{
    // Coefficients
    QString str = QString ( "materiau.coeff_ambiant" );
    GLuint loc = glGetUniformLocation( indiceShader, str.toStdString().c_str() );
    glUniform1f ( loc, this->coeff_ambiant );

    str = QString ( "materiau.coeff_speculaire" );
    loc = glGetUniformLocation( indiceShader, str.toStdString().c_str() );
    glUniform1f ( loc, this->coeff_speculaire );

    str = QString ( "materiau.coeff_diffus" );
    loc = glGetUniformLocation( indiceShader, str.toStdString().c_str() );
    glUniform1f ( loc, this->coeff_diffus );

    // Hardness
    str = QString ( "materiau.hardness" );
    loc = glGetUniformLocation( indiceShader, str.toStdString().c_str() );
    glUniform1i ( loc, this->hardness );

}

// Getters
float Materiau::getCoeffAmbiant ( void )
{
    return this->coeff_ambiant;
}
float Materiau::getCoeffDiffus ( void )
{
    return this->coeff_diffus;
}
float Materiau::getCoeffSpeculaire ( void )
{
    return this->coeff_speculaire;
}
int Materiau::getHardness ( void )
{
    return this->hardness;
}
// Setters
void Materiau::setMateriau ( float amb, float diff, float spec, int hard )
{
    this->coeff_ambiant    = amb;
    this->coeff_diffus     = diff;
    this->coeff_speculaire = spec;
    this->hardness         = hard;
}
