#include "panneauscene.h"

PanneauScene::PanneauScene( void )
{
    // Rien
}

PanneauScene::PanneauScene( Scene* s )
{
    this->setWindowTitle("Scène");
    this->scene = s;

    // Timer
    this->raffraichissement = new QTimer(this);
    QObject::connect( this->raffraichissement, SIGNAL( timeout() ), this, SLOT( mise_a_jour() ) );
    this->raffraichissement->start( 1000 );

    // Les sous-panneaux
    this->creerPanneauFPS();  
    this->creerPanneauObjet();
    this->creerPanneauMateriau();
    this->creerPanneauLumieres();

    // Mise en page des sous-panneaux
    QGridLayout* panneau = new QGridLayout();
    QSpacerItem* ressort = new QSpacerItem( 10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    panneau->addWidget( this->FPS,          0, 0 );
    panneau->addItem ( ressort,             1, 0 );
    panneau->addWidget( this->objet,        2, 0 );
    panneau->addWidget( this->pan_materiau, 3, 0 );
    panneau->addItem ( ressort,             4, 0 );
    panneau->addWidget( this->pan_lumieres, 5, 0 );
    this->setLayout( panneau );
}

PanneauScene::~PanneauScene( void )
{
    // Rien
}

/* Les FPS */
void PanneauScene::creerPanneauFPS ( void )
{
    this->labelFPS = new QLabel("FPS : ");
    this->menuFPS  = new QComboBox();
    this->textFPS  = new QLabel("FPS : 0");
    this->textMs   = new QLabel("Temps : 0.0 ms");

    this->menuFPS->addItem("Restriction 60 FPS");
    this->menuFPS->addItem("Restriction 120 FPS");
    this->menuFPS->addItem("Restriction 144 FPS");
    this->menuFPS->addItem("Sans restriction");

    QGridLayout* layoutFPS = new QGridLayout();
    layoutFPS->addWidget( this->labelFPS, 0, 0 );
    layoutFPS->addWidget( this->textFPS,  1, 0 );
    layoutFPS->addWidget( this->textMs,   1, 1 );
    layoutFPS->addWidget( this->menuFPS,  1, 2 );

    this->FPS = new QWidget();
    FPS->setLayout(layoutFPS);

    // Signaux
    QObject::connect( this->menuFPS, SIGNAL( currentIndexChanged(int) ), this, SLOT( changerFPS() ) );
}

void PanneauScene::changerFPS ( void )
{
    unsigned int n = 0;
    QStringList list = this->menuFPS->currentText().split(" ", QString::SkipEmptyParts);
    for ( int i = 0 ; i < list.size() ; i++ )
    {
        if ( list.at(i).toInt() > 0 ) // return 0 en cas d'erreur
        {
            n = list.at(i).toInt();
        }
    }
    this->scene->setRestrictedFPS(n);
}

void PanneauScene::mise_a_jour( void )
{
    QString str = QString("");
    unsigned int n = this->scene->getFPS();
    str += QString::number(n);
    str += " FPS";
    this->textFPS->setText( str );

    str = QString("Temps : ");
    str += QString::number( 1000.0 / n );
    str += "ms";
    this->textMs->setText(str);
}

/* Les objets de la scène */
void PanneauScene::creerPanneauObjet ( void )
{
    this->labelObjet          = new QLabel("Objets Quads");
    this->menuObjet           = new QComboBox();
    this->labelObjetTriangles = new QLabel("Objets Triangles");
    this->menuObjetTriangles  = new QComboBox();
    this->chargerObjet        = new QPushButton( "Charger" );
    this->suppObjet           = new QPushButton( "Supprimer" );

    QGridLayout* layoutObj = new QGridLayout();
    layoutObj->addWidget( this->labelObjet,            0, 0 );
    layoutObj->addWidget( this->menuObjet,             0, 1 );
    layoutObj->addWidget( this->labelObjetTriangles,   1, 0 );
    layoutObj->addWidget( this->menuObjetTriangles,    1, 1 );
    layoutObj->addWidget( this->chargerObjet,          2, 0 );
    layoutObj->addWidget( this->suppObjet,             2, 1 );

    this->objet = new QWidget();
    this->objet->setLayout(layoutObj);

    // Récupération des objets
    for ( unsigned int i = 0 ; i < this->scene->getNbObjets( type_quads ) ; i++ )
    {
        this->menuObjet->addItem( this->scene->getObjet( type_quads, i ).getNom() );
    }
    for ( unsigned int i = 0 ; i < this->scene->getNbObjets( type_triangles ) ; i++ )
    {
        this->menuObjetTriangles->addItem( this->scene->getObjet( type_triangles, i ).getNom() );
    }

    // Les signaux
    QObject::connect( this->menuObjet,          SIGNAL( currentIndexChanged(int) ), this, SLOT( changerObjet() ) );
    QObject::connect( this->menuObjet,          SIGNAL( highlighted(int) ),         this, SLOT( changerObjet() ) );
    QObject::connect( this->menuObjetTriangles, SIGNAL( currentIndexChanged(int) ), this, SLOT( changerObjet() ) );
    QObject::connect( this->menuObjetTriangles, SIGNAL( highlighted(int) ),         this, SLOT( changerObjet() ) );

    QObject::connect( this->scene,              SIGNAL( changementTypeAutorise()),  this, SLOT( changerObjet() ) );

    QObject::connect( this->chargerObjet,       SIGNAL( clicked() ),                this, SLOT( addObjet() ) );
    QObject::connect( this->suppObjet,          SIGNAL( clicked() ),                this, SLOT( delObjet() ) );
}

void PanneauScene::addObjet( void )
{
    // TODO A modifier
    QString path_fichier = QFileDialog::getOpenFileName
    (
        this,
        "Selection du maillage",
        this->scene->getPath() + "/objets",
        ""
    );
    this->scene->addObjet( path_fichier );
    this->menuObjet->addItem( this->scene->getObjetCourant().getNom() );
    this->menuObjet->setCurrentIndex( this->menuObjet->count()-1 );
}
void PanneauScene::delObjet( void )
{
    this->scene->delObjet();
    this->menuObjet->removeItem( this->menuObjet->currentIndex() );
}
void PanneauScene::changerObjet( void )
{
    if ( sender() == this->menuObjet )
    {
        this->scene->setObjetCourant( type_quads, this->menuObjet->currentIndex() );
    }
    else if ( sender() == this->menuObjetTriangles )
    {
        this->scene->setObjetCourant( type_triangles, this->menuObjetTriangles->currentIndex() );
    }
    else if ( sender() == this->scene )
    {
        switch ( this->scene->getTypeAutorise() )
        {
            case type_quads :
            {
                this->menuObjet->setEnabled( true );
                this->menuObjetTriangles->setEnabled( false );
                this->scene->setObjetCourant( type_quads, this->menuObjet->currentIndex() );
            }
            break;
            case type_triangles :
            {
                this->menuObjet->setEnabled( false );
                this->menuObjetTriangles->setEnabled( true );
                this->scene->setObjetCourant( type_triangles, this->menuObjetTriangles->currentIndex() );
            }
            break;
            case type_quelconque :
            {
                this->menuObjet->setEnabled( true );
                this->menuObjetTriangles->setEnabled( true );
            }
            break;
        }
    }
    Materiau materiau = this->scene->getObjetCourant().getMateriau();
    this->spin_ambiant->setValue(    int( 100 * materiau.getCoeffAmbiant()    ) );
    this->spin_diffus->setValue(     int( 100 * materiau.getCoeffDiffus()     ) );
    this->spin_speculaire->setValue( int( 100 * materiau.getCoeffSpeculaire() ) );
    this->spin_hardness->setValue(   materiau.getHardness()                     );
}

// Le matériau
void PanneauScene::creerPanneauMateriau( void )
{
    this->label_materiau   = new QLabel ( "Matériau :" );
    this->label_ambiant    = new QLabel ( "Ambiante" );
    this->label_diffus     = new QLabel ( "Diffus" );
    this->label_speculaire = new QLabel ( "Speculaire" );
    this->label_hardness   = new QLabel ( "Hardness" );

    this->spin_ambiant    = new QSpinBox ();
    this->spin_diffus     = new QSpinBox ();
    this->spin_speculaire = new QSpinBox ();
    this->spin_hardness   = new QSpinBox ();

    this->spin_hardness->setMinimum(1); // Pour ne pas avoir de puissance 0

    // Mise en page
    QGridLayout* layout = new QGridLayout();

    layout->addWidget( this->label_materiau,   0, 0, 1, 2 );
    layout->addWidget( this->label_ambiant,    1, 0 );
    layout->addWidget( this->label_diffus,     1, 2 );
    layout->addWidget( this->label_speculaire, 2, 0);
    layout->addWidget( this->label_hardness,   2, 2 );
    layout->addWidget( this->spin_ambiant,     1, 1 );
    layout->addWidget( this->spin_diffus,      1, 3 );
    layout->addWidget( this->spin_speculaire,  2, 1 );
    layout->addWidget( this->spin_hardness,    2, 3 );

    QObject::connect( this->spin_ambiant,    SIGNAL(valueChanged(int)), this, SLOT(changerMateriau()) );
    QObject::connect( this->spin_diffus,     SIGNAL(valueChanged(int)), this, SLOT(changerMateriau()) );
    QObject::connect( this->spin_speculaire, SIGNAL(valueChanged(int)), this, SLOT(changerMateriau()) );
    QObject::connect( this->spin_hardness,   SIGNAL(valueChanged(int)), this, SLOT(changerMateriau()) );

    this->pan_materiau = new QWidget();
    this->pan_materiau->setLayout( layout );
    this->changerObjet(); // Pour charger le premier matériau
}

void PanneauScene::changerMateriau( void )
{
    this->scene->getObjetCourant().getMateriau().setMateriau
    (
        (double)(this->spin_ambiant->value())/100,
        (double)(this->spin_diffus->value())/100,
        (double)(this->spin_speculaire->value())/100,
        this->spin_hardness->value()
    );
}

// Les lumières
void PanneauScene::creerPanneauLumieres ( void )
{
    this->titre_lumiere = new QLabel( "Lumières" );
    this->choix_lumiere = new QComboBox();
    this->nb_lumieres = 0;
    QString str = QString ( "Lumière " );
    for ( unsigned int i = 0 ; i < this->scene->getNbLumieres() ; i++ )
    {
        this->choix_lumiere->addItem( str + QString::number( this->nb_lumieres ) );
        nb_lumieres++;
    }
    QObject::connect( this->choix_lumiere, SIGNAL(currentIndexChanged(int)), this, SLOT(changerLumiereCourante(int)) );
    this->label_puiss = new QLabel ( "Puissance" );
    this->spin_puiss = new QSpinBox();

    // Position
    this->label_pos = new QLabel ( "Position" );
    this->spin_posX = new QDoubleSpinBox();
    this->spin_posY = new QDoubleSpinBox();
    this->spin_posZ = new QDoubleSpinBox();
    this->spin_posX->setMinimum(-5);
    this->spin_posX->setMaximum( 5);
    this->spin_posY->setMinimum(-5);
    this->spin_posY->setMaximum( 5);
    this->spin_posZ->setMinimum(-5);
    this->spin_posZ->setMaximum( 5);
    this->spin_posX->setSingleStep( 0.1 );
    this->spin_posY->setSingleStep( 0.1 );
    this->spin_posZ->setSingleStep( 0.1 );

    // Couleur
    this->label_coul = new QLabel ( "Couleur" );
    this->spin_rouge = new QSpinBox();
    this->spin_vert = new QSpinBox();
    this->spin_bleu = new QSpinBox();
    this->spin_rouge->setMinimum(  0  );
    this->spin_rouge->setMaximum( 255 );
    this->spin_vert->setMinimum (  0  );
    this->spin_vert->setMaximum ( 255 );
    this->spin_bleu->setMinimum (  0  );
    this->spin_bleu->setMaximum ( 255 );

    this->add_lumiere = new QPushButton ( "Ajouter" );
    this->del_lumiere = new QPushButton ( "Supprimer" );
    QObject::connect( this->add_lumiere, SIGNAL(pressed()), this, SLOT(addLumiere()) );
    QObject::connect( this->del_lumiere, SIGNAL(pressed()), this, SLOT(delLumiere()) );

    QGridLayout* layout = new QGridLayout();

    layout->addWidget( this->titre_lumiere, 0, 0, 1, 2 );
    layout->addWidget( this->choix_lumiere, 0, 2, 1, 2 );
    layout->addWidget( this->label_puiss,   1, 0, 1, 2 );
    layout->addWidget( this->spin_puiss,    1, 2, 1, 2 );

    layout->addWidget( this->label_pos, 2, 0 );
    layout->addWidget( this->spin_posX, 2, 1 );
    layout->addWidget( this->spin_posY, 2, 2 );
    layout->addWidget( this->spin_posZ, 2, 3 );

    layout->addWidget( this->label_coul, 3, 0 );
    layout->addWidget( this->spin_rouge, 3, 1 );
    layout->addWidget( this->spin_vert,  3, 2 );
    layout->addWidget( this->spin_bleu,  3, 3 );

    layout->addWidget( this->add_lumiere, 4, 0, 1, 2 );
    layout->addWidget( this->del_lumiere, 4, 2, 1, 2 );

    this->pan_lumieres = new QWidget();
    this->pan_lumieres->setLayout(layout);
    this->changerLumiereCourante(0);

    // Les modification de la lumiere courante
    QObject::connect( this->spin_posX,  SIGNAL(valueChanged(double)), this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_posY,  SIGNAL(valueChanged(double)), this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_posZ,  SIGNAL(valueChanged(double)), this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_rouge, SIGNAL(valueChanged(int)),    this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_vert,  SIGNAL(valueChanged(int)),    this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_bleu,  SIGNAL(valueChanged(int)),    this, SLOT(modificationLumiere()) );
    QObject::connect( this->spin_puiss, SIGNAL(valueChanged(int)),    this, SLOT(modificationLumiere()) );
}

void PanneauScene::changerLumiereCourante ( int n )
{
    this->scene->setLumiereCourante(n);
    Lumiere lum = this->scene->getLumiereCourante();
    this->spin_posX->setValue ( lum.getPosX()       );
    this->spin_posY->setValue ( lum.getPosY()       );
    this->spin_posZ->setValue ( lum.getPosZ()       );
    this->spin_rouge->setValue( lum.getRouge()      );
    this->spin_vert->setValue ( lum.getVert()       );
    this->spin_bleu->setValue ( lum.getBleu()       );
    this->spin_puiss->setValue( lum.getPuissance()  );
}

void PanneauScene::modificationLumiere( void )
{
    Lumiere& lum = this->scene->getLumiereCourante();
    lum.setPosition( this->spin_posX->value(), this->spin_posY->value(), this->spin_posZ->value() );
    lum.setCouleur( this->spin_rouge->value(), this->spin_vert->value(), this->spin_bleu->value() );
    lum.setPuissance( this->spin_puiss->value() );
}

void PanneauScene::addLumiere( void )
{
    if ( this->scene->addLumiere() )
    {
        QString str ( "Lumière ");
        this->choix_lumiere->addItem( str + QString::number( this->nb_lumieres ) );
        this->nb_lumieres++;
    }
    this->choix_lumiere->setCurrentIndex( this->scene->getIndexLumiereCourante() );
}

void PanneauScene::delLumiere( void )
{
    if ( this->scene->delLumiere() )
    {
        this->choix_lumiere->removeItem( this->choix_lumiere->currentIndex() );
    }
    this->choix_lumiere->setCurrentIndex( this->scene->getIndexLumiereCourante() );
}
