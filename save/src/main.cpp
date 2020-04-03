#include "main.h"

/* NVIDIA
extern "C"
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
//
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
/* AMD
extern "C"
{
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
// */

int main ( int argc, char **argv )
{
/*
    Loop schema = Loop();
    std::vector < float > coeff = std::vector < float >();
    for ( unsigned int i = 3 ; i < 4 ; i++ )
    {
        schema.calculCombinaison( coeff, 6 );
    }
    unsigned int count = 0;
    for ( unsigned int i = 3 ; i < 4 ; i++ )
    {
        float somme = 0.0f;
        std::cout << i << "                   ";
        for ( unsigned j = 0 ; j < 12 ; j++ )
        {
            somme += coeff[count];
            std::cout << coeff[count++] << " ; ";
        }
        std::cout << "     :      "  << somme << std::endl;
    }*/

    QApplication app ( argc, argv );
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QString titre = "Subdiv";
    Fenetre* fen = new Fenetre( titre );
    fen->show();
    return app.exec();
}
