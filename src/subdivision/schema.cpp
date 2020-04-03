#include "schema.h"

// Constructeur
Schema::Schema()
{
    this->liste_matrices = std::vector < std::vector<Matrice> >();
    this->dictionnaire   = std::vector < Mot > ();

    this->liste_coeff   = std::vector<float>();
    this->liste_coeff_u = std::vector<float>();
    this->liste_coeff_v = std::vector<float>();
}

// Accesseurs
const Matrice& Schema::getMatrice ( unsigned int nom, unsigned int valence ) const
{
    return this->liste_matrices.at(valence).at(nom);
}

QString Schema::getNom ( void ) const
{
    return this->nom;
}

// Le calcul des coefficients
void Schema::genererCombinaisons ( unsigned int valence_min, unsigned int valence_max )
{
    if ( this->liste_coeff.size() <= 0 )
    {
        this->genererMatrices( valence_min, valence_max );
        this->genererDictionnaire();

        // On génère l'ensemble des mots
        //std::vector < Mot > dictionnaire = std::vector < Mot > ();


        // On génère l'ensemble des combinaisons ( mots x valence )
        for ( unsigned int k = valence_min ; k <= valence_max ; k++ )
        {
            for ( unsigned int i = 0 ; i < this->dictionnaire.size() ; i++ )
            {
                this->calculCombinaison( k, this->dictionnaire.at(i) );
            }
        }
    }
}

// Les accès par fichiers
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
        std::cout << "Erreur lors de l'écriture du fichier : " << path.toStdString() << std::endl;
    }
}
void Schema::lireCombinaisons( QString path ) // Le schéma récupère ses combinaisons depuis son fichier
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
        std::cout << "Erreur lors de la lecture du fichier : " << path.toStdString() << std::endl;
    }
}
void Schema::ecrireTangentesU( QString path ) const
{
    QFile file ( path );
    if ( file.open ( QIODevice::WriteOnly ) )
    {
        QTextStream flux (&file);
        for ( unsigned int i = 0 ; i < this->liste_coeff_u.size() ; i++ )
        {
            flux << this->liste_coeff_u.at(i) << " ";
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de l'écriture du fichier : " << path.toStdString() << std::endl;
    }
}
void Schema::lireTangentesU( QString path )
{
    QFile file ( path );
    if ( file.open ( QIODevice::ReadOnly ) )
    {
        QTextStream flux (&file);
        QString coeff;
        while ( !flux.atEnd() )
        {
            flux >> coeff;
            this->liste_coeff_u.push_back( coeff.toFloat() );
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de la lecture du fichier : " << path.toStdString() << std::endl;
    }
}
void Schema::ecrireTangentesV( QString path ) const
{
    QFile file ( path );
    if ( file.open ( QIODevice::WriteOnly ) )
    {
        QTextStream flux (&file);
        for ( unsigned int i = 0 ; i < this->liste_coeff_v.size() ; i++ )
        {
            flux << this->liste_coeff_v.at(i) << " ";
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de l'écriture du fichier : " << path.toStdString() << std::endl;
    }
}
void Schema::lireTangentesV( QString path )
{
    QFile file ( path );
    if ( file.open ( QIODevice::ReadOnly ) )
    {
        QTextStream flux (&file);
        QString coeff;
        while ( !flux.atEnd() )
        {
            flux >> coeff;
            this->liste_coeff_v.push_back( coeff.toFloat() );
        }
        file.close();
    }
    else
    {
        std::cout << "Erreur lors de la lecture du fichier : " << path.toStdString() << std::endl;
    }
}

// Pour envoyer au shader les informations importantes
void Schema::envoiShader( void ) const
{
    GLuint buffer_combinaisons;
    glGenBuffers( 1, &buffer_combinaisons );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, buffer_combinaisons );
    glBufferData( GL_SHADER_STORAGE_BUFFER, liste_coeff.size() * sizeof( GLfloat ), &(liste_coeff[0]), GL_STATIC_DRAW );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 4, buffer_combinaisons );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ); // unbind

    GLuint buffer_tangentes_u;
    glGenBuffers( 1, &buffer_tangentes_u );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, buffer_tangentes_u );
    glBufferData( GL_SHADER_STORAGE_BUFFER, liste_coeff_u.size() * sizeof(GLfloat), &(liste_coeff_u[0]), GL_STATIC_DRAW );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 5, buffer_tangentes_u );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ); // unbind

    GLuint buffer_tangentes_v;
    glGenBuffers( 1, &buffer_tangentes_v );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, buffer_tangentes_v );
    glBufferData( GL_SHADER_STORAGE_BUFFER, liste_coeff_v.size() * sizeof(GLfloat), &(liste_coeff_v[0]), GL_STATIC_DRAW );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 6, buffer_tangentes_v );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 ); // unbind

}
