#include <sstream>
#include <iostream>
#include <fstream>
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

namespace PolygonalLibrary
{

bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// test markers correctly stored 
void TestMarkersValidity(const PolygonalMesh& mesh)
{
	for (const auto& [marker, ids] : mesh.MarkerCell0Ds)
    {
        cout << "Marker " << marker << " -> [ ";
        for (const auto& id : ids)
        {
            cout << id << " ";
        }
        cout << "]\n";
    }
}

// test lunghezza edges non-zero 
bool TestLengthValidity(const PolygonalMesh& mesh)
{ 	
	for (unsigned int i = 0; i < mesh.NumCell1Ds; i++)
	{	
		double err = 1.0e-16;
		int length = mesh.Cell1DsExtrema(0, i) - mesh.Cell1DsExtrema(1, i);
		if (abs(length) < err)
		{	
			cerr << "length zero" << endl;
			return false;
		}
	}		
	cout << "Tutti i lati hanno lunghezza non zero" << endl;
	return true;
}

// test area poligoni non-zero
bool TestAreaValidity(const PolygonalMesh& mesh)
{ 	
	vector<unsigned int> v; // 
	
	for (unsigned int i = 0; i < mesh.NumCell2Ds; i++)
	{	
		v = mesh.Cell2DsVertices[i];
		unsigned int m = v.size();
		double a = 0.0;
		for (unsigned int j = 0; j < m; j++)
		{
			if (j != m-1)
			{
				double x1 = mesh.Cell0DsCoordinates(0, v[j]); // coor x j per come è def cell0dscoordinates
				double y1 = mesh.Cell0DsCoordinates(1, v[j]); // coor y j 
				double x2 = mesh.Cell0DsCoordinates(0, v[j+1]); // coor x j+1 per come è def cell0dscoordinates
				double y2 = mesh.Cell0DsCoordinates(1, v[j+1]); // coor y j+1
					
				a += 0.5 * abs(x1*y2 - x2*y1);
			}
			else 
			{	double x1 = mesh.Cell0DsCoordinates(0, v[j]); // coor x j per come è def cell0dscoordinates
				double y1 = mesh.Cell0DsCoordinates(1, v[j]); // coor y j 
				double x2 = mesh.Cell0DsCoordinates(0, v[0]); // coor x j+1 per come è def cell0dscoordinates
				double y2 = mesh.Cell0DsCoordinates(1, v[0]); // coor y j+1
				a += 0.5 * abs(x1*y2 - x2*y1);
			}
			
		}
		//cout << a << endl;
		double err = 1.0e-6;
		if (abs(a) < err)
		{	
			cerr << "area zero" << endl;
			return false;
		}
	
			
	}
		
	cout << "Tutti i poligoni hanno area non zero" << endl;
	return true;
}



// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line); //aggiunta alla fine della lista

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0) // se il file non contiene almeno una cella0D do errore
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsID.reserve(mesh.NumCell0Ds); // reserve stabilisce la capacità necessaria per il vettore
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);
	//mesh.Cell0DsMarker.reserve(mesh.NumCell0Ds); 
	//mesh.MarkerCell0Ds = map<unsigned int, list<unsigned int>>;

    for (const string& line : listLines)
    {
        istringstream converter(line); // istringstream legge id, marker, coord x e y 
		stringstream s(line);
		string value;
		
		getline(s, value, ';');
		unsigned int id = stoi(value);
		mesh.Cell0DsID.push_back(id);
		//mesh.Cell0DsCoordinates(2, id) = id;
		
		getline(s, value, ';'); //  marker
		unsigned int marker = stoi(value);
		mesh.Cell0DsMarker.push_back(marker);
		getline(s, value, ';');
		mesh.Cell0DsCoordinates(0, id) = stod(value);
		getline(s, value, ';');
		mesh.Cell0DsCoordinates(1, id) = stod(value);
		
	// Parte dedicata a memorizzare i marker
		if (marker !=0) //marker interni
		{
			auto it = mesh.MarkerCell0Ds.find(marker); // cerca dentro la mappa MarkerCell0Ds in base alla chiave e restituisce l'iterator dell'elemento se la chiave esiste o altrimenti restituisce end  se non lha trovato
			if(it!= mesh.MarkerCell0Ds.end()) 
			{
				(*it).second.push_back(id); // 
			}
			else
			{
				mesh.MarkerCell0Ds.insert({marker, {id}});
			}
		}
    }
    return true;
}
// ***************************************************************************


bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsID.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {	istringstream converter(line); // istringstream legge id, marker, coord x e y 
		stringstream s(line);
		string value;
		
		getline(s, value, ';');
		unsigned int id = stoi(value);
		mesh.Cell1DsID.push_back(id);
		
		getline(s, value, ';'); 
		unsigned int marker = stoi(value);
		mesh.Cell1DsMarker.push_back(marker);
		getline(s, value, ';');
		mesh.Cell1DsExtrema(0, id) = stoi(value);
		getline(s, value, ';');
		mesh.Cell1DsExtrema(1, id) = stoi(value);

	/// Memorizza i marker
		if (marker !=0) //marker interni
			{
				auto it = mesh.MarkerCell1Ds.find(marker); // cerca dentro la mappa MarkerCell0Ds in base alla chiave e restituisce l'iterator dell'elemento se la chiave esiste o altrimenti restituisce end  se non lha trovato
				if(it!= mesh.MarkerCell1Ds.end())
				{
					(*it).second.push_back(id); // 
				
				}
				else
				{
					mesh.MarkerCell1Ds.insert({marker, {id}});
				}
			}
		//}
	
    }

    return true;
}

// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");


    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
	{
        listLines.push_back(line);
	}

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsID.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
		istringstream converter(line); // istringstream legge id, marker, coord x e y 
		stringstream s(line);
		string value;
		
		getline(s, value, ';');
		unsigned int id = stoi(value);
		mesh.Cell2DsID.push_back(id);
		
		getline(s, value, ';'); // salto marker
		
		getline(s, value, ';');
		unsigned int nvertices = stoi(value);
		vector<unsigned int> vertices;
		vertices.resize(nvertices);
		for(unsigned int i = 0; i < nvertices; i++)
		{	
			getline(s, value, ';');
			vertices[i]= stoi(value);
		}
		getline(s, value, ';');
		unsigned int nedges = stoi(value);
		vector<unsigned int> edges;
		edges.resize(nedges);
		for(unsigned int i = 0; i < nedges; i++)
		{	
			getline(s, value, ';');
			edges[i]= stoi(value);
		}
		
		mesh.Cell2DsVertices.push_back(vertices);
		mesh.Cell2DsEdges.push_back(edges);
	}
        
    return true;
}



}


