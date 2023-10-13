#ifndef FENETRE_H
#define FENETRE_H

#include "../core.h"
#include "widgetopengl.h"
#include "panneaulateral.h"
#include "panneauscene.h"
#include "../rendu/scene.h"
#include "../controls/keyboard.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QScreen>

//#include <windows.h>
#include <GL/glew.h>
//#include <GL/freeglut.h>

class Fenetre : public QMainWindow
{
    Q_OBJECT

    private :
        PanneauLateral* pan_core;
        PanneauScene*   pan_scene;
        QDockWidget* dockGauche;
        QDockWidget* dockDroite;

    public :
        Fenetre( QString titre, Keyboard* keys );

    public slots :
        void toggleLeftDock();
        void toggleRightDock();
        void quitApp();
};

#endif // FENETRE_H
