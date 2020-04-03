#include "shaderprogram.h"

// Constructeur / destructeur
ShaderProgram::ShaderProgram( void )
{
    this->on_off = false;
}
ShaderProgram::~ShaderProgram( void )
{
    // glDeleteProgram ( this->indice );
}

// Accesseurs
GLuint ShaderProgram::getIndice ( void ) const
{
    return this->indice;
}
bool ShaderProgram::isActivated ( void ) const
{
    return this->on_off;
}
void ShaderProgram::addShader ( Shader* s )
{
    glAttachShader( this->indice, s->getIndice() );
}
void ShaderProgram::delShader ( Shader* s )
{
    glDetachShader( this->indice, s->getIndice() );
}

// Les accesseurs aux attributs du shader
void ShaderProgram::setVertexAttribut ( const char* nom, GLint nb, GLenum type, GLsizei stride ) const
{
    GLint attribut = glGetAttribLocation( this->indice, nom );
    glEnableVertexAttribArray( attribut );
    switch ( type )
    {
        // Si l'attribut est entier
        case GL_INT :
        case GL_UNSIGNED_INT :
        case GL_BYTE :
        case GL_UNSIGNED_BYTE :
        case GL_SHORT :
        case GL_UNSIGNED_SHORT :
        {
            glVertexAttribIPointer ( attribut, nb, type, stride, 0 );
        }
        break;

        // S'il est double
        case GL_DOUBLE :
        {
            glVertexAttribLPointer ( attribut, nb, type, stride, 0 );
        }
        break;

        // S'il est float (cas par défault)
        default :
        {
            glVertexAttribPointer ( attribut, nb, type, GL_FALSE, stride, 0 );
        }
        break;
    }
}

void ShaderProgram::setVector(const char* nom, const std::vector<GLfloat>& tableau ) const
{
    GLint localisation = glGetUniformLocation( this->indice, nom );
    glUniform1fv( localisation, (GLsizei)(tableau.size()), &tableau[0] );
}

void ShaderProgram::setMat4x4( const char* nom, const glm::mat4& valeur ) const
{
    glUniformMatrix4fv( glGetUniformLocation( this->indice, nom ), 1, GL_FALSE, &valeur[0][0] );
}

// La vie du ShaderProgram
void ShaderProgram::create ( void )
{
    this->indice = glCreateProgram ( );
}
void ShaderProgram::link ( void )
{
    glLinkProgram ( this->indice );
}
void ShaderProgram::activer( QString* str )
{
    glUseProgram( this->indice );
    this->on_off = true;
    str->push_back( "Le shader est activé \n" );
}
void ShaderProgram::desactiver( QString* str )
{
    glUseProgram( 0 );
    this->on_off = false;
    str->push_back( "Le shader est désactivé \n" );
}
void ShaderProgram::changerEtat ( QString* str )
{
    if ( this->on_off )
    {
        this->desactiver( str );
    }
    else
    {
        this->activer( str );
    }
}

// Les méthodes statiques
void ShaderProgram::infoGPU ( void )
{
    std::cout << "***** Info GPU *****" << std::endl;
    std::cout << "Fabricant : " << glGetString (GL_VENDOR) << std::endl;
    std::cout << "Carte graphique: " << glGetString (GL_RENDERER) << std::endl;
    std::cout << "Version : " << glGetString (GL_VERSION) << std::endl;
    std::cout << "Version GLSL : " << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;

    /* Version QString qui ne marche pas
    const GLubyte* vendeur = glGetString (GL_VENDOR);
    const char* vend = reinterpret_cast<const char*>( vendeur );
    QString s = "";
    s += "*************** Info GPU ***************";
    s += "\nFabricant               : ";
    s += QString::fromUtf8(vend);
    s += "\nCarte graphique : ";
    s += reinterpret_cast<const char*>( glGetString (GL_RENDERER) );
    s += "\nVersion                  : ";
    s += reinterpret_cast<const char*>( glGetString (GL_VERSION) );
    s += "\nVersion GLSL       : ";
    s += reinterpret_cast<const char*>( glGetString (GL_SHADING_LANGUAGE_VERSION) );
    return s;
    */
}
