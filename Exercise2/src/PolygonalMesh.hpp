#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh
{	
	unsigned int NumCell0Ds = 0; // num punti
	unsigned int NumCell1Ds = 0; // num segmenti
	unsigned int NumCell2Ds = 0; // num poligoni 
	
	vector<unsigned int> Cell0DsID = {}; //registra ID punti
	vector<unsigned int> Cell1DsID = {};
	vector<unsigned int> Cell2DsID = {};
	
	Eigen::MatrixXd Cell0DsCoordinates = {}; 
	Eigen::MatrixXi Cell1DsExtrema = {};
	vector<vector<unsigned int>> Cell2DsVertices = {};
	vector<vector<unsigned int>> Cell2DsEdges = {};
	
	map<unsigned int, list<unsigned int>> MarkerCell0Ds = {};
	map<unsigned int, list<unsigned int>> MarkerCell1Ds = {};
	vector<unsigned int> Cell0DsMarker = {};
	vector<unsigned int> Cell1DsMarker = {};
};

}