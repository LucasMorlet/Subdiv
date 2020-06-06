#include "fenetre.h"

Fenetre::Fenetre( QString titre, Keyboard* keys ) : QMainWindow( NULL )
{
    // La fenêtre
    this->setWindowTitle( titre );
    /*
    QDesktopWidget bureau;
    int x = 0.05 * bureau.width();
    int y = 0.05 * bureau.height();
    int w = 0.9 * bureau.width();
    int h = 0.7 * bureau.height();
    */

    int bureauWidth  = GetSystemMetrics ( SM_CXSCREEN );
    int bureauHeight = GetSystemMetrics ( SM_CYSCREEN );
    int x = 0.05 * bureauWidth;
    int y = 0.05 * bureauHeight;
    int w = 0.9 * bureauWidth;
    int h = 0.7 * bureauHeight;
    this->setGeometry(x, y, w, h);

    // Initialisation du core
    Core* core = new Core();
    Scene* scene = new Scene();

    // Le Widget central
    WidgetOpenGL* widg = new WidgetOpenGL( core, scene );
    this->setCentralWidget( widg );

    // Les deux panneaux latéraux   
    this->pan_scene = new PanneauScene( scene );
    this->dockGauche = new QDockWidget( this, 0 );
    this->dockGauche->setWidget( pan_scene );
    this->dockGauche->setFixedWidth( 300 );
    this->addDockWidget( Qt::LeftDockWidgetArea, this->dockGauche );

    this->pan_core = new PanneauLateral( core, scene );
    this->dockDroite = new QDockWidget( this, 0 );
    this->dockDroite->setWidget( pan_core );
    this->dockDroite->setFixedWidth( 300 );
    this->addDockWidget( Qt::RightDockWidgetArea, this->dockDroite );

    QObject::connect( keys, SIGNAL(toggleLeftDock()), this, SLOT(toggleLeftDock()));
    QObject::connect( keys, SIGNAL(toggleRightDock()), this, SLOT(toggleRightDock()));
    QObject::connect( keys, SIGNAL(quitApp()), this, SLOT(quitApp()));
}

void Fenetre::toggleLeftDock()
{
    if ( this->dockGauche->isVisible() ) this->dockGauche->hide();
    else this->dockGauche->show();
}

void Fenetre::toggleRightDock()
{
    if ( this->dockDroite->isVisible() ) this->dockDroite->hide();
    else this->dockDroite->show();
}

void Fenetre::quitApp()
{
    this->close();
}
