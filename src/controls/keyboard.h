#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QKeyEvent>

class Keyboard : public QObject
{   
    Q_OBJECT

    public:
        Keyboard();
        bool clavier ( int touche );

    protected :
        bool eventFilter( QObject* object, QEvent* event ) override;

    signals :
        void toggleLeftDock();
        void toggleRightDock();
        void quitApp();
};

#endif // KEYBOARD_H
