#include "shader.h"

Shader::Shader ( void )
{
    // Constructeur par défaut qui ne sert à rien à part aux initialisations vides
}

Shader::Shader( GLenum t, QString p, QString* erreur )
{
    // Les attributs
    this->type = t;
    this->path = p;
    QFileInfo info ( this->path );

    // Création du shader
    this->indice = glCreateShader( type );
    if ( this->indice == 0 )
    {
        erreur->push_back( "Erreur lors de la création du shader " + info.fileName() + "\n" );
    }
    else
    {
        QFile fichier( this->path );
        // Ouverture du fichier
        if ( !fichier.open(QFile::ReadOnly | QFile::Text) )
        {
            erreur->push_back( "Erreur lors de l'ouverture du fichier " + info.absoluteFilePath() + "\n" );
            glDeleteShader( this->indice );
        }
        else
        {
            // Lecture du contenu du fichier
            QTextStream in( &fichier );
            QString contenu = in.readAll();
            std::string test = contenu.toStdString();
            GLchar* src = (GLchar*)test.c_str();
            glShaderSource( this->indice, 1, (const GLchar**)&src, NULL );
            glCompileShader( this->indice );

            // Test de la compilation du shader
            GLint ok;
            glGetShaderiv( this->indice, GL_COMPILE_STATUS, &ok );
            if ( ok != GL_TRUE )
            {
                GLint log_size;
                glGetShaderiv( this->indice, GL_INFO_LOG_LENGTH, &log_size);
                GLchar* log = new GLchar[ log_size + 1 ];
                glGetShaderInfoLog( this->indice, log_size, &log_size, log);

                erreur->push_back( "Erreur à la compilation du shader " + info.fileName() + "\n" + log +"\n" );
                delete log;
                glDeleteShader( this->indice );
            }
            else
            {
                erreur->push_back( info.fileName() + " OK \n" );
            }
        }
    }
}

Shader::~Shader ( void )
{
     glDeleteShader( this->indice );
}

GLuint Shader::getIndice ( void ) const
{
    return this->indice;
}
