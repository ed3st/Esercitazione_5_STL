#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary 
{	
bool ImportMesh(PolygonalMesh& mesh)
{
	if ( !ImportCell0D(mesh))
		return false;
	
	if ( !ImportCell1D(mesh))
		return false;
	
	if ( !ImportCell2D(mesh))
		return false;
	
	//Stampo i marker per controllare che siano stati memorizzati correttamente.
	//Marker Cell0D
	cout << "Cell0D markers:" << endl;
	PrintMarkers(mesh.Cell0DMarker);
	
	//Marker Cell1D
	cout << "Cell1D markers:" << endl;
	PrintMarkers(mesh.Cell1DMarker);
	
	//Marker Cell2D
	cout << "Cell2D markers:" << endl;
	PrintMarkers(mesh.Cell2DMarker);
	
	return true;
}
// ********************************************************************
bool ImportCell0D(PolygonalMesh& mesh)
{
	ifstream file("./Cell0Ds.csv"); //Apro Cell0Ds.csv
	
	if (file.fail()) //Controllo che l'apertura sia andata a buon fine
		return false;
	
	list<string> listLines;
	string line;
	while (getline(file, line))
		listLines.push_back(line);
	/*listLines è ora una lista di stringe contenente (come stringe) tutte le righe del file.*/
	
	file.close(); //Chiudo Cell0Ds.csv
	
	listLines.pop_front();//Rimuovo l'header
	
	mesh.NumCell0D = listLines.size(); //Numero di celle 0D
	
	//Controllo che ci siano celle 0D
	if (mesh.NumCell0D == 0) {
		cerr << "There is no cell 0D." << endl;
		return false;
	};
	
	mesh.Cell0DId.reserve(mesh.NumCell0D); //Aggiorno opportunamente la capacità del vettore.
	mesh.Cell0DCoord = MatrixXd::Zero(3,mesh.NumCell0D); //Qui salverò le cordinate (x,y) dei punti. (z=0 siamo in 2D)
	// "Riempio" mesh.Cell0DId, mesh.Cell0DCoord e mesh.Cell0DMarker 
	for(const string& line : listLines) 
	{
		istringstream converter(line);
		
		char tmp; //Variabile temporanea che sfrutto per leggere ";"
		unsigned int id;
		unsigned int marker;
		
		converter >> id >> tmp >> marker >> tmp >> mesh.Cell0DCoord(0, id) >> tmp >> mesh.Cell0DCoord(1, id);
		
		mesh.Cell0DId.push_back(id);
		
		/*Memorizzazione del marker. Se è diverso da zero, procedo con la memorizzazione. 
		Controllo se è tra i marker già trovati: 
		- se no, lo aggiungo e gli associo il vertice id;
		- se si, aggiungo id alla lista dei vertici associati.*/
		if  (marker != 0) 
		{
			const auto it = mesh.Cell0DMarker.find(marker);
			if (it == mesh.Cell0DMarker.end()) 
			{
				mesh.Cell0DMarker.insert({marker,{id}}); 
			}
			else 
			{
				it->second.push_back(id); 
			}
		}
	}
	return true;
}
// ********************************************************************
bool ImportCell1D(PolygonalMesh& mesh)
{
	ifstream file("./Cell1Ds.csv"); //Apro Cell1Ds.csv
	
	if (file.fail()) //Controllo che l'apertura sia andata a buon fine
		return false;
	
	list<string> listLines;
	string line;
	while (getline(file, line))
		listLines.push_back(line);
	/*listLines è ora una lista di stringe contenente (come stringe) tutte le righe del file.*/
	
	file.close(); //Chiudo Cell1Ds.csv
	
	listLines.pop_front(); //Rimuovo l'header
	
	mesh.NumCell1D = listLines.size(); //Numero di celle 1D
	
	//Controllo che ci siano celle 1D.
	if ( mesh.NumCell1D == 0 ) {
		cerr << "There is no cell 1D." << endl;
		return false;
	};
	
	mesh.Cell1DId.reserve(mesh.NumCell1D);  //Aggiorno opportunamente la capacità del vettore.
	mesh.Cell1DExtrema = MatrixXi::Zero(2,mesh.NumCell1D); //Qui salverò gli id dei punti estremi dei lati.
	
	/*Variabile booleana che tiene traccia della presenza o meno di lati di lunghezza nulla. 
	Se notNull = true non ci sono lati di lunghezza nulla, altrimenti notNull = false.*/
	bool notNull = true; 
	
	// "Riempio" mesh.Cell1DId, mesh.Cell1DExtrema e mesh.Cell1DMarker 
	for(const string& line : listLines) 
	{
		istringstream converter(line);
		
		char tmp;
		unsigned int id;
		unsigned int marker;
		Vector2i vertices;
		
		converter >> id >> tmp >> marker >> tmp >> vertices(0) >> tmp >> vertices(1);
		
		//Aggiungo id e estremi alla mesh
		mesh.Cell1DId.push_back(id);
		mesh.Cell1DExtrema(0, id) = vertices(0);
		mesh.Cell1DExtrema(1, id) = vertices(1);
		
		/*Memorizzazione del marker. Se è diverso da zero, procedo con la memorizzazione. 
		Controllo se è tra i marker già trovati: 
		- se no, lo aggiungo e gli associo il lato id;
		- se si, aggiungo id alla lista dei lati associati.*/
		if ( marker != 0 )
		{
			const auto it = mesh.Cell1DMarker.find(marker); 
			if ( it == mesh.Cell1DMarker.end() ) 
			{
				mesh.Cell1DMarker.insert({marker,{id}});
			}
			else 
			{
				it->second.push_back(id); 
			}
		}
		
		//Check sulla lunchezza del lato
		double lengthEdge;
		const Vector3d& originCoord = mesh.Cell0DCoord.col(vertices(0));
		const Vector3d& endCoord = mesh.Cell0DCoord.col(vertices(1));
		lengthEdge = pow((originCoord(0) - endCoord(0)),2) + pow((originCoord(1) - endCoord(1)),2); //Calcolo la lunghezza del lato con il teorema di Pitagora
		lengthEdge = sqrt(lengthEdge);
		if (lengthEdge < 1e-16) {
			notNull = false;
			cout << "Edge with length 0 found. Edge id: " << id << " ." << endl;
		}	
		
	}
	
	//Stampo eventuale esito positivo del test sulla lunghezza del lato. 
	if (notNull)
		cout << "All edges have non-zero length." << endl;
	
	return true;
}
// ********************************************************************
bool ImportCell2D(PolygonalMesh& mesh)
{
	ifstream file("./Cell2Ds.csv"); //Apro Cell2Ds.csv
	
	if (file.fail()) { //Controllo che l'apertura sia andata a buon fine
		return false;
	}
	
	list<string> listLines;
	string line;
	while (getline(file, line)) 
		listLines.push_back(line);
	
	file.close(); //Chiudo Cell2ds.csv
	
	listLines.pop_front(); //Rimuovo l'header
	
	mesh.NumCell2D = listLines.size(); //Numero di celle 2D
	
	//Controllo che ci siano celle 2D
	if (mesh.NumCell2D == 0) { 
		cerr << "There is no cell 2D." << endl;
		return false;
	}
	
	//Aggiorno opportunamente la capacità dei vettori
	mesh.Cell2DId.reserve(mesh.NumCell2D);
	mesh.Cell2DNumV.reserve(mesh.NumCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumCell2D);
	mesh.Cell2DNumE.reserve(mesh.NumCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumCell2D);
	
	/*Variabile booleana che tiene traccia della presenza o meno di poligoni di area nulla. 
	Se notNull = true non ci sono poligoni di area nulla, altrimenti notNull = false.*/
	bool notNull = true; 

    for (const string& line : listLines)
    {
        istringstream converter(line);
		
		char tmp;
        unsigned int id;
		unsigned int marker;
		unsigned int numV;
		vector<unsigned int> vertices;
		unsigned int numE;
		vector<unsigned int> edges;
		
		converter >> id >> tmp >> marker >> tmp >> numV >> tmp;
		
		vertices.reserve(numV); //Aggiorno la capacità del vettore
        for(unsigned int i = 0; i < numV; i++) 
		{
			unsigned int vertex;
            converter >> vertex >> tmp;
			vertices.push_back(vertex);
		}
		
		converter >> numE;
		edges.reserve(numE); //Aggiorno la capacità del vettore
        for(unsigned int i = 0; i < numE; i++)
		{
            unsigned int edge;
            converter >> tmp >> edge ;
			edges.push_back(edge);
		}

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DNumV.push_back(numV);
		mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DNumE.push_back(numE);
		mesh.Cell2DEdges.push_back(edges);
		
		/*Memorizzazione del marker. Se è diverso da zero, procedo con la memorizzazione. 
		Controllo se è tra i marker già trovati: 
		- se no, lo aggiungo e gli associo il poligono id;
		- se si, aggiungo id alla lista dei poligoni associati.*/
		if ( marker != 0 )
		{
			const auto it = mesh.Cell2DMarker.find(marker);
			if ( it == mesh.Cell2DMarker.end() ) 
			{
				mesh.Cell2DMarker.insert({marker,{id}});
			}
			else 
			{
				it->second.push_back(id);
			}
		}

		//Check sull'area. Per calcolare l'area scompongo il poligono in triangoli.
		double area = 0.0;
		const MatrixXd& coord = mesh.Cell0DCoord;
		for (unsigned int v = 0; v <= numV-1; v++) {
			unsigned int w = (v+1)%numV; //"Chiudo" il poligono
			double x1 = coord(0, v);
			double y1 = coord(1, v);
			double x2 = coord(0, w);
			double y2 = coord(1, w);
			area += x1*y2-x2*y1;
		}
		area = abs(area)*0.5;
		if (area < 1e-16) {
			cerr << "Polygon with null area found.Polygon id: " << id << " ." << endl;
			notNull = false;
		}
    }
	
	//Stampo eventuale esito positivo del check sull'area
	if (notNull) {
		cout << "All polygons have non-zero area." << endl;
	}
	
	return true;
}

void PrintMarkers(const map<unsigned int, list<unsigned int>>& m) 
{
	//Controllo se ci sono marker diversi da zero.
	if (m.size() == 0){
		cout << "No non-zero markers found." << endl;
		return;
	}
	//Stampo i marker diversi da zero con id associati.
	for (const auto& [k, v] : m) 
	{
		cout << "\t" << k << " : { ";
		unsigned int j = 0; //Contatore che uso per sapere quando stampare la virgola
		for (const auto& w : v) 
		{
			if (j > 0)
				cout << ", ";
			
			cout << w;
			j++;
		}
		cout << " }" << endl;
	}
}

}