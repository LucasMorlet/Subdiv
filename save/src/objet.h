#ifndef OBJET_H
#define OBJET_H

#include "shaderprogram.h"
#include "texture.h"
#include "materiau.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

// Pour le loader
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class Objet
{
    private :

        // Pour la gestion des différents modèles
        QString nom;

        // Les 9 degrés de libertés
        GLfloat posX, posY, posZ;
        GLfloat scaleX, scaleY, scaleZ;
        GLfloat roll, pitch, yaw;

        // Matériau, texture
        Materiau materiau;

        // La construction du modèle classique
        std::vector<GLfloat> sommets;         // Les sommets du maillage original (x,y,z)
        std::vector<GLfloat> textures;        // (u,v)
        std::vector<GLfloat> normales;        // (nx,ny,nz)
        std::vector<GLuint>  indices_faces;   // Indexation des sommets pour les faces du maillage de départ

        unsigned int type_face;
        GLuint buff_sommets, buff_textures, buff_normales, buff_indices;   // Les différents buffeurs que l'on va utiliser pour envoyer les infos au GPU

        // Ce qui est spécifique aux subdivisions
        bool isPatch;
        GLuint nb_patchs;

        // La construction de type patch
        std::vector<GLfloat> sommets_patchs;    // Si nécessaire les sommets après une vraie subdivision, sinon identique à ceux du maillage de contrôle
        std::vector<GLuint>  faces_patchs;      // Les faces du maillage de contrôle =/= maillage de départ (on a pivoté certaines faces) pour le LoD dynamique
        std::vector<GLuint>  patchs;            // Le patch qui indexe les sommets
        std::vector<GLuint>  patchs_indexes;    // Les infos pour chaque patch (index_debut, taille)

        GLuint buff_sommets_patchs_depart, buff_sommets_patchs, buff_faces_patchs, buff_patchs, buff_patchs_indexes; // Les différents buffeurs spécifique aux patchs

    public :
        // Constructeurs
        Objet ( void );
        Objet ( QString fich );
        // Accesseurs
        QString getNom( void ) const;
        glm::vec3 getPosition( void ) const;
        glm::mat4 getMatriceModel( void ) const;
        std::vector<GLuint> getIndices ( void ) const;
        std::vector<GLfloat> getSommets( void ) const;
        void setSommetsPatchs ( const std::vector<GLfloat> &ind );
        void setFacesPatchs ( const std::vector<GLuint> &ind );
        void setIndicesPatchs ( const std::vector<GLuint> &ind );
        void setPatchs ( const std::vector<GLuint> &ind );
        unsigned int getTypeFace ( void ) const;
        unsigned int getNbFaces ( void ) const;
        void setTypePatch( bool b );
        void setNbPatchs( unsigned int i );
        Materiau& getMateriau( void );
        // Affichage
        void initVBO( void );
        void afficher ( ShaderProgram& shader ) const;
        void afficher ( void ) const;
        // Debug
        void toString ( void );

    private :
        void rendu( void ) const;
};

#endif // OBJET_H