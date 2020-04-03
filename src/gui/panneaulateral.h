#ifndef PANNEAULATERAL_H
#define PANNEAULATERAL_H

#include "../core.h"
#include "../rendu/shader.h"
#include "../rendu/shaderprogram.h"
#include "../rendu/objet.h"
#include "../rendu/scene.h"

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QCoreApplication>
#include <QDir>
#include <QWidget>
#include <QFileDialog>
#include <QTextEdit>
#include <QSpinBox>

class PanneauLateral : public QWidget
{
    Q_OBJECT

    private :
        // Le core
        Core* core;
        Scene* scene;  

        // Panneau schémas de subdivision
        QWidget*   schema;
        QLabel*    labelSchema;
        QComboBox* menuSchema;
        QLabel*    labelSubdiv;
        QSpinBox*  spinSubdiv;

        // Panneau shader
        QWidget* shader;

        QLabel*      labelShader;
        QCheckBox*   activer;
        QPushButton* compiler;

        QLabel*      labelVert;
        QCheckBox*   activerVert;
        QComboBox*   menuVert;

        QLabel*      labelTCS;
        QCheckBox*   activerTCS;
        QComboBox*   menuTCS;

        QLabel*      labelTES;
        QCheckBox*   activerTES;
        QComboBox*   menuTES;

        QLabel*      labelGeom;
        QCheckBox*   activerGeom;
        QComboBox*   menuGeom;

        QLabel*      labelFrag;
        QCheckBox*   activerFrag;
        QComboBox*   menuFrag;

        QTextEdit*   texteShader;

    public :
        PanneauLateral( Core* c, Scene* s );
        ~PanneauLateral( void );

    private :       
        void creerPanneauSchema ( void );
        void creerPanneauShader ( void );

    private slots :
        void initShaders ( void );        
        void changerSchema ( int index );
        void changerNiveauSubdiv ( int niveau );

};

#endif // PANNEAULATERAL_H
