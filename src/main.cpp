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
    QString titre = "Subdiv";
    Fenetre* fen = new Fenetre( titre );
    fen->show();
    return app.exec();
}
