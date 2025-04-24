#pragma once

#include <iostream>
#include "Eigen/Eigen"

#include <vector>
#include <list>
#include <map>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary 
{
struct PolygonalMesh
{
	// Cell0D
	unsigned int NumCell0D = 0; //numero di Cell0D
	vector<unsigned int> Cell0DId = {}; //Cell0D id, size 1 x NumCell0D
	MatrixXd Cell0DCoord = {}; //Cell0D coordinates , size 2 x NumCell0D
	map<unsigned int, list<unsigned int>> Cell0DMarker = {}; //Cell0D markers
	
	// Cell1D
	unsigned int NumCell1D = 0; //numero di Cell1D
	vector<unsigned int> Cell1DId = {}; //Cell1D id, size 1 x NumCell1D
	MatrixXi Cell1DExtrema = {}; //Cell1D origin and end, size 2 x NumCell1D
	map<unsigned int, list<unsigned int>> Cell1DMarker = {}; //Cell1D markers
	
	
	// Cell2D
	unsigned int NumCell2D = 0; //numero di Cell2D
	vector<unsigned int> Cell2DId = {}; //Cell2D id, size 1 x NumCell2D
	vector<unsigned int> Cell2DNumV = {}; //numero di vertici per Cell2D, size 1 x NumCell2D
	vector<vector<unsigned int>> Cell2DVertices = {}; //Cell2D vertici
	vector<unsigned int> Cell2DNumE = {}; //Cell2D numero di lati
	vector<vector<unsigned int>> Cell2DEdges = {}; //Cell2D lati
	map<unsigned int, list<unsigned int>> Cell2DMarker = {}; //Cell2D markers
};
};
