#ifndef WIDGETOPENGL_H
#define WIDGETOPENGL_H

#include "../core.h"
#include "../rendu/scene.h"
#include "../rendu/objet.h"
#include "../rendu/shaderprogram.h"
#include "../subdivision/schema.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <vector>
#include <cmath>

class WidgetOpenGL : public QOpenGLWidget
{
    Q_OBJECT

    private :
        int startX, startY;

        Scene* scene;
        Core* core;

    public:
        WidgetOpenGL ( Core* c, Scene* s );
        void initializeGL();
        void paintGL();
        void resizeGL( int w, int h );
        void mousePressEvent( QMouseEvent* event );
        void mouseMoveEvent( QMouseEvent* event );
        void mouseReleaseEvent( QMouseEvent* event );
};

#endif // WIDGETOPENGL_H
