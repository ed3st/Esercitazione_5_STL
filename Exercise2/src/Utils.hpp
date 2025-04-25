#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
/*ImportMesh importa una mesh poligonale e ne testa la correttezza;
restituisce il risultato della lettura: true se ha avuto successo, false altrimenti.
mesh è una struct di tipo PolygonalMesh.*/
bool ImportMesh(PolygonalMesh& mesh);

/*ImportCell0D importa dal file Cell0Ds.csv e
restituisce il risultato della lettura: true se ha avuto successo, false altrimenti.
mesh è una struct di tipo PolygonalMesh.*/
bool ImportCell0D(PolygonalMesh& mesh);

/*ImportCell1D importa dal file Cell1Ds.csv e
restituisce il risultato della lettura: true se ha avuto successo, false altrimenti.
mesh è una struct di tipo PolygonalMesh.*/
bool ImportCell1D(PolygonalMesh& mesh);

/*ImportCell2D importa dal file Cell2Ds.csv e
restituisce il risultato della lettura: true se ha avuto successo, false altrimenti.
mesh è una struct di tipo PolygonalMesh.*/
bool ImportCell2D(PolygonalMesh& mesh);

/*PrintMarkers stampa i marker e gli id associati.
- m è una mappa le cui chiavi sono i marker, ad ogni chiave(marker) è associata una lista contente fli id associati.*/
void PrintMarkers(const map<unsigned int, list<unsigned int>>& m);
}
