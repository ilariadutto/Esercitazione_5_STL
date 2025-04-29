#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

namespace PolygonalLibrary
{
/// Import the triangular mesh and test if the mesh is correct
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh);

/// Import the Cell0D properties from Cell0Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
// bool ImportCell0Ds(const string& InputFilePath, PolygonalMesh& mesh);
bool ImportCell0Ds(PolygonalMesh& mesh);

/// Import the Cell1D properties from Cell1Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh);

/// Import the Cell2D properties from Cell2Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh);

void TestMarkersValidity(const PolygonalMesh& mesh);

bool TestLengthValidity(const PolygonalMesh& mesh);

bool TestAreaValidity(const PolygonalMesh& mesh);
}
