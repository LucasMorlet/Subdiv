#include "panneaulateral.h"

PanneauLateral::PanneauLateral( Core* c, Scene* s )
{
    this->setWindowTitle("Subdivision Core");
    this->core = c;
    this->scene = s;

    // Initialisation des différents panneaux
    this->creerPanneauSchema();
    this->creerPanneauShader();

    // Mise en page finale
    QGridLayout* panneau = new QGridLayout();    
    panneau->addWidget( this->schema, 2, 0 );
    panneau->addWidget( this->shader, 3, 0 );
    this->setLayout( panneau );
}

PanneauLateral::~PanneauLateral( void )
{
    // Que dalle, Qt supprime de lui-même tous les pointeurs sur les widgets..
}


void PanneauLateral::creerPanneauSchema ( void )
{
    // ******** Le panneau schéma **********
    this->labelSchema = new QLabel("Schéma");
    this->menuSchema = new QComboBox();
    this->labelSubdiv = new QLabel("Niveau de subdivision");
    this->spinSubdiv = new QSpinBox();
    this->spinSubdiv->setMinimum( 0 );
    this->spinSubdiv->setMaximum( 5 );

    QGridLayout* layoutSchema = new QGridLayout();
    layoutSchema->addWidget( this->labelSchema, 0, 0 );
    layoutSchema->addWidget( this->menuSchema,  0, 1 );
    layoutSchema->addWidget( this->labelSubdiv, 1, 0 );
    layoutSchema->addWidget( this->spinSubdiv,  1, 1 );

    this->schema = new QWidget();
    this->schema->setLayout(layoutSchema);

    // Récupération des schémas
    // TODO le faire en automatique sur le core
    this->menuSchema->addItem( "Aucun" );
    for ( unsigned int i = 1 ; i < this->core->getNbSchemas() ; i++ )
    {
        this->menuSchema->addItem( this->core->getSchema(i)->getNom() );
    }

    // Signaux
    QObject::connect( this->menuSchema, SIGNAL( currentIndexChanged(int) ), this, SLOT( changerSchema(int) ) );
    QObject::connect( this->spinSubdiv, SIGNAL( valueChanged(int)),          this, SLOT( changerNiveauSubdiv(int)) );
}
void PanneauLateral::creerPanneauShader ( void )
{
    // ******* Le panneau shader *********
    this->labelShader = new QLabel("Shader");
    this->activer     = new QCheckBox();
    this->compiler    = new QPushButton("Compiler");
    this->labelVert   = new QLabel("Vertex");
    this->labelTCS    = new QLabel("Tesselation Control");
    this->labelTES    = new QLabel("Tesselation Evaluation");
    this->labelGeom   = new QLabel("Geometry");
    this->labelFrag   = new QLabel("Fragment");

    this->menuVert = new QComboBox();
    this->menuTCS  = new QComboBox();
    this->menuTES  = new QComboBox();
    this->menuGeom = new QComboBox();
    this->menuFrag = new QComboBox();

    this->activerVert = new QCheckBox(); this->activerVert->setCheckState( Qt::Checked );
    this->activerTCS  = new QCheckBox(); this->activerTCS ->setCheckState( Qt::Checked );
    this->activerTES  = new QCheckBox(); this->activerTES ->setCheckState( Qt::Checked );
    this->activerGeom = new QCheckBox(); this->activerGeom->setCheckState( Qt::Checked );
    this->activerFrag = new QCheckBox(); this->activerFrag->setCheckState( Qt::Checked );

    QGridLayout* tmp = new QGridLayout();
    tmp->addWidget( this->labelShader, 0, 0 );
    tmp->addWidget( this->activer, 0, 1 );
    this->texteShader = new QTextEdit();
    //TODO this->texteShader->setText( ShaderProgram::infoGPU() );
    QWidget* tmpw = new QWidget();
    tmpw->setLayout(tmp);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget( tmpw, 0, 0 );
    layout->addWidget( this->compiler,    0, 1 );
    layout->addWidget( this->labelVert,   1, 0 );
    layout->addWidget( this->menuVert,    1, 1 );
    layout->addWidget( this->activerVert, 1, 2 );
    layout->addWidget( this->labelTCS,    2, 0 );
    layout->addWidget( this->menuTCS,     2, 1 );
    layout->addWidget( this->activerTCS,  2, 2 );
    layout->addWidget( this->labelTES,    3, 0 );
    layout->addWidget( this->menuTES,     3, 1 );
    layout->addWidget( this->activerTES,  3, 2 );
    layout->addWidget( this->labelGeom,   4, 0 );
    layout->addWidget( this->menuGeom,    4, 1 );
    layout->addWidget( this->activerGeom, 4, 2 );
    layout->addWidget( this->labelFrag,   5, 0 );
    layout->addWidget( this->menuFrag,    5, 1 );
    layout->addWidget( this->activerFrag, 5, 2 );
    layout->addWidget( this->texteShader, 6, 0, 1, 4 );

    this->shader = new QWidget();
    shader->setLayout(layout);

    // Récupération des Shaders
    QDir repertoire = QDir ( this->core->getPath() + "/shaders" );
    QFileInfoList liste = repertoire.entryInfoList();
    for ( int i = 0; i < liste.size() ; i++ )
    {
        if ( liste[i].suffix() == "vert" )
        {
            this->menuVert->addItem( liste[i].baseName() );
        }
        else if ( liste[i].suffix() == "tcs" )
        {
            this->menuTCS->addItem( liste[i].baseName() );
        }
        else if ( liste[i].suffix() == "tes" )
        {
            this->menuTES->addItem( liste[i].baseName() );
        }
        else if ( liste[i].suffix() == "geom" )
        {
            this->menuGeom->addItem( liste[i].baseName() );
        }
        else if ( liste[i].suffix() == "frag" )
        {
            this->menuFrag->addItem( liste[i].baseName() );
        }
    }
    // Les index de base
    this->menuVert->setCurrentIndex( this->menuVert->findText( "colorisation" ) );
    this->menuTCS ->setCurrentIndex( this->menuTCS ->findText( "quads" ) );
    this->menuTES ->setCurrentIndex( this->menuTES ->findText( "quads" ) );
    this->menuGeom->setCurrentIndex( this->menuGeom->findText( "blinn_phong" ) );
    this->menuFrag->setCurrentIndex( this->menuFrag->findText( "blinn_phong" ) );

    // Signaux
    QObject::connect( this->compiler, SIGNAL( clicked() ), this, SLOT( initShaders() ) );
}

void PanneauLateral::initShaders ( void )
{
    this->core->createShader();
    Shader *vert, *tcs, *tes, *geom, *frag;
    QString* erreur = new QString();
    if ( this->activerVert->isChecked() )
    {
        vert = new Shader( GL_VERTEX_SHADER,          this->core->getPath() + "/shaders/" + this->menuVert->currentText() + ".vert", erreur );
        this->core->addShader( vert );
    }
    if ( this->activerTCS ->isChecked() )
    {
        tcs  = new Shader( GL_TESS_CONTROL_SHADER,    this->core->getPath() + "/shaders/" + this->menuTCS->currentText()  + ".tcs",  erreur );
        this->core->addShader( tcs  );
    }
    if ( this->activerTES ->isChecked() )
    {
        tes  = new Shader( GL_TESS_EVALUATION_SHADER, this->core->getPath() + "/shaders/" + this->menuTES->currentText()  + ".tes",  erreur );
        this->core->addShader( tes  );
    }
    if ( this->activerGeom->isChecked() )
    {
        geom = new Shader( GL_GEOMETRY_SHADER,        this->core->getPath() + "/shaders/" + this->menuGeom->currentText() + ".geom", erreur );
        this->core->addShader( geom );
    }
    if ( this->activerFrag->isChecked() )
    {
        frag = new Shader( GL_FRAGMENT_SHADER,        this->core->getPath() + "/shaders/" + this->menuFrag->currentText() + ".frag", erreur );
        this->core->addShader( frag );
    }

    this->core->linkShader();
    this->core->activerShader( erreur );
    this->texteShader->setText( *erreur );

    if ( this->menuSchema->currentIndex() > 0 )
    {
        this->core->getSchemaCourant()->envoiShader();
        this->core->getShader().setUniform( "level", GL_UNSIGNED_INT, this->spinSubdiv->value() );
        this->scene->initVBOs( this->core->getSchemaCourant() );
    }
    else
    {
        this->scene->initVBOs();
    }

    if ( this->activerVert->isChecked() ) this->core->delShader( vert );
    if ( this->activerTCS ->isChecked() ) this->core->delShader( tcs  );
    if ( this->activerTES ->isChecked() ) this->core->delShader( tes  );
    if ( this->activerGeom->isChecked() ) this->core->delShader( geom );
    if ( this->activerFrag->isChecked() ) this->core->delShader( frag );
}
void PanneauLateral::changerSchema( int index )
{
    this->core->setSchemaCourant( index );
    if ( index == Aucun )
    {
        if ( this->scene->getObjetCourant().getTypeFace() == 4 )
        {
            this->menuTCS->setCurrentIndex( this->menuTCS->findText( "quads" ) );
            this->menuTES->setCurrentIndex( this->menuTES->findText( "quads" ) );
        }
        else if ( this->scene->getObjetCourant().getTypeFace() == 3 )
        {
            this->menuTCS->setCurrentIndex( this->menuTCS->findText( "triangles" ) );
            this->menuTES->setCurrentIndex( this->menuTES->findText( "triangles" ) );
        }
        this->scene->setTypeAutorise( type_quelconque );
    }
    else
    {
        this->menuTCS->setCurrentIndex( this->menuTCS->findText( this->core->getSchemaCourant()->getTCS() ) );
        this->menuTES->setCurrentIndex( this->menuTES->findText( this->core->getSchemaCourant()->getTES() ) );
        if ( index == Catmull_Clark || index == Doo_Sabin )
        {
            this->scene->setTypeAutorise( type_quads );
        }
        else if ( index == Loop_enum )
        {
            this->scene->setTypeAutorise( type_triangles );
        }
    }
}
void PanneauLateral::changerNiveauSubdiv( int niveau )
{
    this->core->getShader().setUniform( "level", GL_UNSIGNED_INT, niveau );
}
