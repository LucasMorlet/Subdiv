#include "core.h"

Core::Core()
{
    this->path = QCoreApplication::applicationDirPath();
    this->path.truncate( this->path.lastIndexOf("/") );

    // Les schémas
    this->lesSchemas = std::vector<Schema*>();
    this->lesSchemas.push_back( new CatmullClark() ); // Schéma "Aucun"
    this->lesSchemas.push_back( new CatmullClark() );
    this->lesSchemas.push_back( new DooSabin() );
    this->lesSchemas.push_back( new Loop() );
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
    QString path_fichier( this->path + "/combinaisons/" + this->getSchemaCourant()->getTES() + ".bc" );
    if ( QFile::exists( path_fichier) )
    {
        this->getSchemaCourant()->lireCombinaisons(path_fichier);
    }
    else
    {
        std::cout << "Fichier de combinaisons inexistant -> création en cours" << std::endl;
        this->getSchemaCourant()->genererCombinaisons( 3, 10 );
        this->getSchemaCourant()->ecrireCombinaisons(path_fichier);
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
