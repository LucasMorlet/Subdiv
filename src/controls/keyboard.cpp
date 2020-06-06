#include "keyboard.h"

/* Keyboard class converts KeyEvent to custom signal */

Keyboard::Keyboard()
{

}

bool Keyboard::clavier( int touche )
{
    switch ( touche )
    {
        case Qt::Key_S :
        {
            emit toggleLeftDock();
        }
        break;
        case Qt::Key_C :
        {
            emit toggleRightDock();
        }
        break;
        case Qt::Key_Escape : emit quitApp();
        break;
        default : return false; // La touche n'est pas interceptée
    }
    return true; // La touche a été traitée par le switch -> elle est interceptée
}

bool Keyboard::eventFilter( QObject* object /*cible originale*/, QEvent* event )
{
    // Si le signal est de type KeyPress, on le gère avec cette classe
    if ( event->type() == QEvent::KeyPress )
    {
        return this->clavier( static_cast<QKeyEvent*>(event)->key() );
    }
    // Sinon on le laisse simplement passer
    return false;
}
