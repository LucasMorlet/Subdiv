#ifndef WIDGETOPENGL_H
#define WIDGETOPENGL_H

#include "scene.h"
#include "objet.h"
#include "shaderprogram.h"
#include "schema.h"
#include "core.h"

#include <QGLWidget>
#include <QKeyEvent>
#include <vector>
#include <cmath>

class WidgetOpenGL : public QGLWidget
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
