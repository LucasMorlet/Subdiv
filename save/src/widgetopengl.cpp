#include "widgetopengl.h"

// Constructeur
WidgetOpenGL::WidgetOpenGL ( Core* c , Scene* s )
{
    this->core = c;

    this->scene = s;
    this->scene->getCam().setRatio( (double)(this->width()) / (double)(this->height()) );
    this->scene->creerTimer( this );
    this->scene->lancerTimer();
}

// Méthodes d'affichage
void WidgetOpenGL::initializeGL()
{
    // GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        std::cout << "Erreur de GLEW" << std::endl;
        std::cout << glewGetErrorString( err ) << std::endl;
    }

    // L'initialisation des objets
    this->scene->initVBOs();

    // Les informations de la carte graphique
    ShaderProgram::infoGPU();

    // Les trucs de base OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Classique
    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Rendu d'image
}
void WidgetOpenGL::paintGL()
{
    // La remise à zéro
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // On affiche l'objet courant
    if ( this->core->isShaderActivated() ) // La version shader
    {
        this->scene->afficher( this->core->getShader() );
    }
    else // La version CPU
    {
        this->scene->afficher();
    }

    // La gestion des chronos
    this->scene->testFPS();
}

// Feedbacks
void WidgetOpenGL::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
    float r = (float)w / (float)h;
    this->scene->getCam().setRatio( r );
}

void WidgetOpenGL::mousePressEvent( QMouseEvent* event )
{
    this->startX = event->x();
    this->startY = this->height() - event->y();
}

void WidgetOpenGL::mouseMoveEvent( QMouseEvent* event )
{
    int yy = this->height() - event->y();
    this->scene->getCam().rotate( event->x() - this->startX, yy - this->startY, this->scene->getObjetCourant() );
    this->startX = event->x();
    this->startY = yy;
}

void WidgetOpenGL::mouseReleaseEvent( QMouseEvent* event )
{
    this->scene->getCam().rotate( event->x() - this->startX, this->height() - event->y() - this->startY, this->scene->getObjetCourant() );
}
