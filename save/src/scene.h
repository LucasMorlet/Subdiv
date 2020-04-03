#ifndef SCENE_H
#define SCENE_H

#include "lumiere.h"
#include "camera.h"
#include "objet.h"
#include "schema.h"

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QString>
#include <QCoreApplication>
#include <QDir>

enum type_objet { type_quads, type_triangles, type_quelconque };

class Scene : public QObject
{
    Q_OBJECT // pour émettre des signaux

    private :
        QString path;
        Camera cam;
        std::vector<Lumiere> lesLumieres;
        unsigned int lumiere_courante;

        // Les objets
        std::vector<Objet> lesObjetsQuads;
        std::vector<Objet> lesObjetsTriangles;
        Objet objetCourant;
        type_objet type_autorise;

        // Le minuteur pour le rafraichissement et le compteur de FPS
        QTimer* timer;
        unsigned int FPS_fixed;
        QTime chrono;
        unsigned int nb_frames;
        unsigned int FPS;

    public:
        Scene( void );
        ~Scene ( void );
        void afficher ( void );
        void afficher ( ShaderProgram shader );
        Camera& getCam ( void );
        QString getPath( void );

        // Lumière
        Lumiere& getLumiereCourante ( void );
        unsigned int getIndexLumiereCourante ( void );
        void setLumiereCourante ( unsigned int n );
        unsigned int getNbLumieres ( void );
        bool addLumiere( void );
        bool delLumiere( void );

        // Objet
        void loadObjets ( void );
        void initVBOs ( void );
        void initVBOs ( Schema* s );
        Objet& getObjet ( type_objet type, unsigned int i );
        Objet& getObjetCourant ( void );
        unsigned int getNbObjets ( type_objet type ) const;
        void addObjet ( QString file );
        void delObjet( void );
        void setObjetCourant( type_objet type, unsigned int i );
        void setTypeAutorise ( type_objet t );
        type_objet getTypeAutorise( void );

        // Les FPS
        void creerTimer ( QObject* receveur );
        void lancerTimer( void );
        void setRestrictedFPS( unsigned int n );
        unsigned int getFPS( void );
        void testFPS( void );

    signals :
        void changementTypeAutorise ( void );
};

#endif // SCENE_H
