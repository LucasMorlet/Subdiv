#include "fenetre.h"

Fenetre::Fenetre( QString titre ) : QMainWindow( NULL )
{
    // La fenêtre
    this->setWindowTitle( titre );
    QDesktopWidget bureau;
    int x = 0.05 * bureau.width();
    int y = 0.05 * bureau.height();
    int w = 0.9 * bureau.width();
    int h = 0.7 * bureau.height();
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

    qApp->installEventFilter(this);
}

bool Fenetre::clavier( int touche )
{
    switch ( touche )
    {
        case Qt::Key_S :
        {
            if ( this->dockGauche->isVisible() ) this->dockGauche->hide();
            else this->dockGauche->show();
        }
        break;
        case Qt::Key_C :
        {
            if ( this->dockDroite->isVisible() ) this->dockDroite->hide();
            else this->dockDroite->show();
        }
        break;
        case Qt::Key_Escape : this->close();
        default : return false; // La touche n'est pas interceptée
    }
    return true; // La touche a été traitée par le switch -> elle est interceptée
}

bool Fenetre::eventFilter( QObject* object /*cible originale*/, QEvent* event )
{
    if ( event->type() == QEvent::KeyPress )
    {
        return this->clavier( static_cast<QKeyEvent*>(event)->key() );
    }
    return false; // La cible originale reçoit quand même l'évenement
}
