#ifndef CORE_H
#define CORE_H

#include "rendu/objet.h"
#include "rendu/shaderprogram.h"

#include "subdivision/catmullclark.h"
#include "subdivision/doosabin.h"
#include "subdivision/loop.h"
#include "subdivision/quadratique_non_uniforme.h"

#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include <QTime>

enum { Aucun, Catmull_Clark, Doo_Sabin, Loop_enum, Quadratique_enum };

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
