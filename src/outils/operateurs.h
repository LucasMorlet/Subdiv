#ifndef OPERATEUR_H
#define OPERATEUR_H

#include "vecteur.h"
#include "matrice.h"
#include <Eigen/Eigenvalues>

Matrice operator* ( const Matrice& A, const Matrice& B );
Matrice operator* ( float lambda, const Matrice& M );
Vecteur operator* ( float lambda, const Vecteur& V );
Vecteur operator* ( const Matrice& M, const Vecteur& V );
Vecteur operator* ( const Vecteur& V, const Matrice& M );

#endif // OPERATEUR_H
