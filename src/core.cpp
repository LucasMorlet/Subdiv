#include "core.h"

Core::Core()
{
    // Présent aussi dans la scène
    this->path = QCoreApplication::applicationDirPath();
    this->path.truncate( this->path.lastIndexOf("/") );
    this->path.truncate( this->path.lastIndexOf("/") );

    // Les schémas
    this->lesSchemas = std::vector<Schema*>();
    this->lesSchemas.push_back( new CatmullClark() ); // Schéma "Aucun"
    this->lesSchemas.push_back( new CatmullClark() );
    this->lesSchemas.push_back( new DooSabin() );
    this->lesSchemas.push_back( new Loop() );
    this->lesSchemas.push_back( new Quadratique_non_uniforme() );
    this->schemaCourant = 0;

    // Le Shader Program
    this->progShad = ShaderProgram();
}
QString Core::getPath( void ) const
{
    return this->path;
}

// Les schémas
unsigned int Core::getNbSchemas ( void ) const
{
    return this->lesSchemas.size();
}
Schema* Core::getSchema( unsigned int i ) const
{
    return this->lesSchemas.at( i );
}
Schema* Core::getSchemaCourant( void ) const
{
    return this->lesSchemas.at( this->schemaCourant );
}
void Core::setSchemaCourant( unsigned int index )
{
    this->progShad.desactiver( new QString() );
    this->schemaCourant = index;

    // La lecture ou le calcul/écriture des combinaisons
    QString path_fichier( this->path + "/combinaisons/" + this->getSchemaCourant()->getTES() );
    if ( QFile::exists( path_fichier + ".bc" ) && QFile::exists( path_fichier + ".u" ) && QFile::exists( path_fichier + ".v" ) )
    {
        this->getSchemaCourant()->lireCombinaisons( path_fichier + ".bc" );
        this->getSchemaCourant()->lireTangentesU( path_fichier + ".u" );
        this->getSchemaCourant()->lireTangentesV( path_fichier + ".v" );
    }
    else
    {
        std::cout << "Fichier(s) manquant(s) -> calcul des combinaisons en cours" << std::endl;
        this->getSchemaCourant()->genererCombinaisons( 3, 10 );
        std::cout << "Calcul terminé" << std::endl;

        if ( !QFile::exists( path_fichier + ".bc" ) )
        {
            std::cout << "Fichier de combinaisons inexistant -> création en cours" << std::endl;
            this->getSchemaCourant()->ecrireCombinaisons( path_fichier + ".bc" );
            std::cout << "Fichier de combinaisons crée" << std::endl;
        }
        if ( !QFile::exists( path_fichier + ".u" ) )
        {
            std::cout << "Fichier de tangentes-u inexistant -> création en cours" << std::endl;
            this->getSchemaCourant()->ecrireTangentesU( path_fichier  + ".u" );
            std::cout << "Fichier de tangentes-u crée" << std::endl;
        }
        if ( !QFile::exists( path_fichier + ".v" ) )
        {
            std::cout << "Fichier de tangentes-v inexistant -> création en cours" << std::endl;
            this->getSchemaCourant()->ecrireTangentesV( path_fichier + ".v" );
            std::cout << "Fichier de tangentes-v crée" << std::endl;
        }
    }
}

// Le Shader Program
ShaderProgram& Core::getShader( void )
{
    return this->progShad;
}
void Core::createShader ( void )
{
    this->progShad.create();
}
void Core::addShader ( Shader* s )
{
    this->progShad.addShader(s);
}
void Core::delShader ( Shader* s )
{
    this->progShad.delShader(s);
}
void Core::linkShader ( void )
{
    this->progShad.link();
}
void Core::activerShader ( QString* erreur )
{
    this->progShad.activer( erreur );
}
bool Core::isShaderActivated ( void )
{
    return this->progShad.isActivated();
}
