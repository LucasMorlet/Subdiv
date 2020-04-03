#ifndef CORE_H
#define CORE_H

#include "objet.h"
#include "shaderprogram.h"

#include "catmullclark.h"
#include "doosabin.h"
#include "loop.h"

#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include <QTime>

enum { Aucun, Catmull_Clark, Doo_Sabin, Loop_enum };

class Core
{
    private :
        // Le path de l'application
        QString path;

        // Les schémas
        std::vector<Schema*> lesSchemas; // Doit être des pointeurs car ils sont abstraits
        unsigned int schemaCourant;

        // Le ShaderProgram
        ShaderProgram progShad;

    public:
        Core();
        QString getPath( void ) const;

        Schema* getSchema ( unsigned int i ) const;
        Schema* getSchemaCourant ( void ) const;
        unsigned int getNbSchemas ( void ) const;
        void setSchemaCourant ( unsigned int index );

        ShaderProgram& getShader( void );
        void createShader ( void );
        void addShader ( Shader* s );
        void delShader ( Shader* s );
        void linkShader ( void );
        void activerShader ( QString* erreur );
        bool isShaderActivated ( void );
};

#endif // CORE_H
