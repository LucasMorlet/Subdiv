#include "main.h"

/* NVIDIA
extern "C"
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
//
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}*/
/* AMD
extern "C"
{
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
// */

int main ( int argc, char **argv )
{
    QApplication app ( argc, argv );
    /*QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));*/

    // Keyboard
    Keyboard* keys = new Keyboard();
    qApp->installEventFilter( keys );

    // Main Window
    QString titre = "Subdiv";
    Fenetre* fen = new Fenetre( titre, keys );
    fen->show();

    // Execute app
    int exec = app.exec();
    delete fen;
    delete keys;
    return exec;
}
