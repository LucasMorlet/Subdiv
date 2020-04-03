#include "scene.h"

Scene::Scene( void ) : QObject()
{
    // Le path (présent aussi dans le core)
    this->path = QCoreApplication::applicationDirPath();
    this->path.truncate( this->path.lastIndexOf("/") );
    this->path.truncate( this->path.lastIndexOf("/") );

    // La camera
    this->cam = Camera ( 65.0f );

    // Les lumières
    this->lesLumieres.push_back( Lumiere ( 0.0f, 5.0f, 0.0f, 255, 255, 255, 40 ) );
    this->lumiere_courante = 0;

    // Les objets
    this->lesObjetsQuads = std::vector<Objet>();
    this->lesObjetsTriangles = std::vector<Objet>();
    this->loadObjets();
    this->type_autorise = type_quelconque;

    // Le compteur de FPS
    this->FPS_fixed = 60;
    this->nb_frames = 0;
    this->chrono = QTime();
    this->chrono.start();
}

Scene::~Scene ( void )
{
    // Rien
}

void Scene::afficher ( ShaderProgram shader )
{
    // Le temps
    int ms = 1000*QTime::currentTime().second() + QTime::currentTime().msec();
    shader.setUniform( "time", GL_INT, ms%1000 );

    // La caméra
    Objet* objCourant = this->getObjetCourant();
    if ( objCourant ) this->cam.lookat( *objCourant );

    // Les matrices MVP
    const glm::mat4& Model = objCourant ? objCourant->getMatriceModel() : glm::mat4(1.0);
    const glm::mat4& View  = this->cam.getViewMatrix();
    const glm::mat4& Proj  = this->cam.getProjectionMatrix();
    const glm::mat4  MV    = View * Model;
    const glm::mat3  mv    = glm::mat3 ( MV[0][0], MV[0][1], MV[0][2], MV[1][0], MV[1][1], MV[1][2], MV[2][0], MV[2][1], MV[2][2] );
    shader.setMat4x4( "MVP",  Proj*MV );
    shader.setMat4x4( "MV",   MV );
    shader.setMat3x3( "tiMV", glm::transpose( glm::inverse(mv) ) );

    /* Les lumières fixes */
    for ( unsigned int i = 0 ; i < this->lesLumieres.size() ; i++ )
    {
        this->lesLumieres[i].sendToShader( shader.getIndice(), View, i );
    }
    // Fin lumières fixes */

    /* Les lumières animées *
    double anim_lum = (double)(ms%3000) / 3000;
    for ( unsigned int i = 0 ; i < this->lesLumieres.size() ; i++ )
    {
        double theta = ( (anim_lum + i*0.333) * 2.0 * 3.141592 );
        this->lesLumieres[i].setPosition( 2.0*sin(theta), 1.0, 2.0*cos(theta) );
        this->lesLumieres[i].sendToShader( shader, View, i );
    }
    // Fin lumières animées */

    // On comble les lumières restantes avec des lumières noires
    // pour que GLSL "oublie" les valeurs déjà écrites
    for ( unsigned int i = this->lesLumieres.size() ; i < 10 ; i++ )
    {
        Lumiere::Black_Light().sendToShader( shader.getIndice(), View, i );
    }

    if ( objCourant ) objCourant->afficher( shader );
}

void Scene::afficher ( void )
{
    // Une vieille fonction qu'il faut supprimer
    // this->cam.CPULookAt( this->getObjetCourant() );
    // this->getObjetCourant().afficher();
}

Camera& Scene::getCam ( void )
{
    return this->cam;
}

const QString& Scene::getPath( void )
{
    return this->path;
}


// Lumière
Lumiere& Scene::getLumiereCourante ( void )
{
    return this->lesLumieres.at( this->lumiere_courante );
}

unsigned int Scene::getIndexLumiereCourante ( void )
{
    return this->lumiere_courante;
}

void Scene::setLumiereCourante ( unsigned int n )
{
    this->lumiere_courante = n;
}

unsigned int Scene::getNbLumieres ( void )
{
    return this->lesLumieres.size();
}

bool Scene::addLumiere( void )
{
    if ( this->lesLumieres.size() < 10 ) // Pas plus de 10 lumières
    {
        this->lesLumieres.push_back( Lumiere() );
        this->lumiere_courante = this->lesLumieres.size()-1;
        return true;
    }
    return false;
}

bool Scene::delLumiere( void )
{
    if ( this->lesLumieres.size() > 1 ) // On a toujours une lumière active
    {
        this->lesLumieres.erase( this->lesLumieres.begin() + this->lumiere_courante );
        this->lumiere_courante = 0;
        return true;
    }
    return false;
}

// Les objets
void Scene::loadObjets ( void )
{
    QDir repertoire = QDir ( this->path + "/objets" );
    QFileInfoList liste = repertoire.entryInfoList();
    for ( int i = 0; i < liste.size() ; i++ )
    {
        if ( liste[i].suffix() == "obj" )
        {
            this->addObjet( liste[i].filePath() );
        }
    }
}
void Scene::initVBOs ( void )
{
    for ( unsigned int i = 0 ; i < this->lesObjetsQuads.size() ; i++ )
    {
        this->lesObjetsQuads[i].setTypePatch( false );
        this->lesObjetsQuads[i].initVBO();
    }
    for ( unsigned int i = 0 ; i < this->lesObjetsTriangles.size() ; i++ )
    {
        this->lesObjetsTriangles[i].setTypePatch( false );
        this->lesObjetsTriangles[i].initVBO();
    }
    this->type_courant = type_quads;
    this->index_courant = 0;
}

void Scene::initVBOs ( Schema* s )
{
    if ( s->getNom() == "Catmull-Clark" || s->getNom() == "Doo-Sabin" )
    {
        for ( unsigned int i = 0 ; i < this->lesObjetsQuads.size() ; i++ )
        {
            this->lesObjetsQuads[i].setTypePatch( true ); // A mettre dans le découpage
            s->faces2patches( this->lesObjetsQuads[i] );
            this->lesObjetsQuads[i].initVBO();
        }
    }
    else if ( s->getNom() == "Loop" )
    {
        for ( unsigned int i = 0 ; i < this->lesObjetsTriangles.size() ; i++ )
        {
            this->lesObjetsTriangles[i].setTypePatch( true );
            s->faces2patches( this->lesObjetsTriangles[i] );
            this->lesObjetsTriangles[i].initVBO();
        }
    }
}

Objet* Scene::getObjet ( type_objet type, unsigned int i )
{   
    switch ( type )
    {
        case type_quads :
        {
            if ( this->lesObjetsQuads.size() > i ) return &(this->lesObjetsQuads[i]);
            else return nullptr;
        }
        case type_triangles :
        {
            if ( this->lesObjetsTriangles.size() > i ) return &(this->lesObjetsTriangles[i]);
            else return nullptr;
        }
        case type_quelconque :
        {
            return nullptr;
        }
        default : return nullptr;
    }
}

Objet* Scene::getObjetCourant ( void )
{
    return this->getObjet ( this->type_courant, this->index_courant );
}
unsigned int Scene::getNbObjets ( type_objet type ) const
{
    if ( type == type_quads )
    {
        return this->lesObjetsQuads.size();
    }
    else if ( type == type_triangles )
    {
        return this->lesObjetsTriangles.size();
    }
    return 0;
}
void Scene::addObjet ( QString file )
{
    Objet obj = Objet ( file );
    if ( obj.getTypeFace() == 3 )
    {
        this->lesObjetsTriangles.push_back( obj );
        this->index_courant = lesObjetsTriangles.size() - 1 ;
    }
    else if ( obj.getTypeFace() == 4 )
    {
        this->lesObjetsQuads.push_back( obj );
        this->index_courant = this->lesObjetsQuads.size() - 1 ;
    }
}
void Scene::delObjet( void )
{
    // TODO
}
void Scene::setObjetCourant( type_objet type, unsigned int i )
{
    this->type_courant = type;
    this->index_courant = i;
}
void Scene::setTypeAutorise( type_objet t )
{
    this->type_autorise = t;
    emit changementTypeAutorise(); // On prévient le panneau que certains objets peuvent être inaccessible et le panneau nous dira le nouvel objet courant
}
type_objet Scene::getTypeAutorise( void )
{
    return this->type_autorise;
}

// Les FPS
void Scene::creerTimer ( QObject* receveur )
{
    this->timer = new QTimer( receveur );
    QObject::connect( this->timer, SIGNAL( timeout() ), receveur, SLOT( update() ) );
}

void Scene::lancerTimer()
{
    // On réactive le minuteur
    if ( this->FPS_fixed <= 0 )
    {
        this->timer->start( 0 );
    }
    else
    {
        this->timer->start( 1000 / this->FPS_fixed );
    }
}

void Scene::testFPS( void )
{
    // Le calcul du nombre de FPS
    this->nb_frames++;
    if ( this->chrono.elapsed() > 1000 )
    {
        this->FPS = this->nb_frames;
        this->nb_frames = 0;
        this->chrono.restart();
    }
}

void Scene::setRestrictedFPS( unsigned int n )
{
    this->FPS_fixed = n;
    this->lancerTimer();
}

unsigned int Scene::getFPS( void )
{
    return this->FPS;
}

