#ifndef PANNEAUSCENE_H
#define PANNEAUSCENE_H

#include "scene.h"

#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>

class PanneauScene : public QWidget
{
    Q_OBJECT

    private :
        Scene* scene;
        QTimer* raffraichissement;

        // Panneau FPS
        QWidget* FPS;
        QLabel* labelFPS;
        QComboBox* menuFPS;
        QLabel* textFPS;
        QLabel* textMs;

        // Panneau objet
        QWidget* objet;
        QLabel* labelObjet;
        QComboBox* menuObjet;
        QLabel* labelObjetTriangles;
        QComboBox* menuObjetTriangles;
        QPushButton* chargerObjet;
        QPushButton* suppObjet;

        // Panneau matériau
        QWidget* pan_materiau;
        QLabel* label_materiau;
        QLabel* label_ambiant;
        QLabel* label_speculaire;
        QLabel* label_diffus;
        QLabel* label_hardness;
        QSpinBox* spin_ambiant;
        QSpinBox* spin_speculaire;
        QSpinBox* spin_diffus;
        QSpinBox* spin_hardness;

        // Panneau lumière
        unsigned int    nb_lumieres;
        QWidget*        pan_lumieres;
        QLabel*         titre_lumiere;
        QPushButton*    add_lumiere;
        QPushButton*    del_lumiere;
        QComboBox*      choix_lumiere;
        QLabel*         label_puiss;
        QSpinBox*       spin_puiss;
        QLabel*         label_pos;
        QDoubleSpinBox* spin_posX;
        QDoubleSpinBox* spin_posY;
        QDoubleSpinBox* spin_posZ;
        QLabel*         label_coul;
        QSpinBox*       spin_rouge;
        QSpinBox*       spin_vert;
        QSpinBox*       spin_bleu;

    public:
        PanneauScene( void );
        PanneauScene( Scene* s );
        ~PanneauScene( void );

    private :
        void creerPanneauFPS      ( void );
        void creerPanneauObjet    ( void );
        void creerPanneauMateriau ( void );
        void creerPanneauLumieres ( void );

    private slots :
        void changerFPS ( void );
        void mise_a_jour( void );
        void addObjet( void );
        void delObjet( void );
        void changerObjet  ( void );
        void changerMateriau ( void );
        void changerLumiereCourante ( int n );
        void modificationLumiere ( void );
        void addLumiere ( void );
        void delLumiere ( void );
};

#endif // PANNEAUSCENE_H
