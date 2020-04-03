#ifndef OPERATEUR_H
#define OPERATEUR_H

#include "vecteur.h"
#include "matrice.h"
#include <Eigen/Eigenvalues>

Matrice operator* (   Matrice A, Matrice B );
Matrice operator* ( float lambda, Matrice M );
Vecteur operator* ( float lambda, Vecteur V );
Vecteur operator* ( Matrice M, Vecteur V );
Vecteur operator* ( Vecteur V, Matrice M );

#endif // OPERATEUR_H
