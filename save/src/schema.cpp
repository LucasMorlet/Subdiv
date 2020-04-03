#include "schema.h"

Schema::Schema()
{
    this->liste_coeff = std::vector<float>();
    this->liste_matrices = std::vector < std::vector<Matrice> >();
}

Matrice& Schema::getMatrice ( unsigned int nom, unsigned int valence )
{
    return this->liste_matrices.at(valence).at(nom);
}

QString Schema::getNom ( void ) const
{
    return this->nom;
}

void Schema::calculCombinaison( unsigned int valence, Mot mot  )
{
    Matrice M = this->Identity( valence ); // Matrice identité pour chaque schéma
    for ( unsigned int i = 0 ; i < mot.size() ; i++ )
    {
        M = this->getMatrice( mot.at(i), valence ) * M;
        if ( mot.at(i) != T0 )
        {
            valence = this->valence_ordinaire;
        }
    }
    for ( unsigned int i = 0 ; i < M.getNbColonnes() ; i++ )
    {
        this->liste_coeff.push_back( M.getCoeff( 0, i ) );
    }
 }
void Schema::ecrireCombinaisons( QString path ) const
{
    QFile file ( path );
    if ( file.open ( QIODevice::WriteOnly ) )
    {
        QTextStream flux (&file);
        for ( unsigned int i = 0 ; i < this->liste_coeff.size() ; i++ )
        {
            flux << this->liste_coeff.at(i) << " ";
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de l'écriture du fichier de combinaisons : " << path.toStdString() << std::endl;
    }
}
void Schema::lireCombinaisons( QString path )
{
    QFile file ( path );
    if ( file.open ( QIODevice::ReadOnly ) )
    {
        QTextStream flux (&file);
        QString coeff;
        while ( !flux.atEnd() )
        {
            flux >> coeff;
            this->liste_coeff.push_back( coeff.toFloat() );
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de la lecture du fichier de combinaisons : " << path.toStdString() << std::endl;
    }
}
void Schema::envoiShader( void ) const
{
    GLuint buffer_combinaisons;
    glGenBuffers( 1, &buffer_combinaisons );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, buffer_combinaisons );
    glBufferData( GL_SHADER_STORAGE_BUFFER, liste_coeff.size() * sizeof(GLuint), &(liste_coeff[0]), GL_STATIC_DRAW );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, buffer_combinaisons );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ); // unbind
}
