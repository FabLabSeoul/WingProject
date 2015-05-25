
#include "stdafx.h"
#include "terrainexporter.h"
#include <fstream>

using namespace graphic;
using namespace graphic::exporter;
using namespace std;


// 파일경로의 역슬래쉬를 슬래쉬로 바꾼다.
string ConvertRS2S(const string &str)
{
	string tmp = str;
	common::replaceAll(tmp, "\\", "/");
	return tmp;
}


// 지형정보를 json 포맷으로 저장한다.
bool exporter::WriteRawTerrainFile( const string &fileName, const sRawTerrain &terrain )
{
	ofstream of(fileName);
	if (!of.is_open())
		return false;

	of << "{" << endl;

	of << "\t\"terrain version\" : \"v1\"," << endl;
	of << "\t\"info\" : " << endl;
	of << "\t{" << endl;
	of << "\t\"rowCellCount\" : " << terrain.rowCellCount << "," << endl;
	of << "\t\"colCellCount\" : " << terrain.colCellCount << "," << endl;
	of << "\t\"cellSize\" : " << terrain.cellSize << "," << endl;
	of << "\t\"heightMapStyle\" : " << terrain.heightMapStyle <<  "," << endl;
	of << "\t\"heightMap\" : " << "\"" << ConvertRS2S(terrain.heightMap) << "\"," << endl;
	of << "\t\"bg texture\" : " << "\"" << ConvertRS2S(terrain.bgTexture) << "\"," << endl;
	of << "\t\"alpha texture\" : " << "\"" << ConvertRS2S(terrain.alphaTexture) << "\"," << endl;
	of << "\t\"alpha texture width\" : " << terrain.alphaTextureWidth << "," << endl;
	of << "\t\"alpha texture height\" : " << terrain.alphaTextureHeight << "," << endl;
	of << "\t\"texture factor\" : " << terrain.textureFactor << "," << endl;
	of << "\t\"height factor\" : " << terrain.heightFactor << "," << endl;
	of << "\t\"render water\" : " << terrain.renderWater << "," << endl;

	of << "\t\"layer\" : [" << endl;
	for (int i=0; i < 4; ++i)
	{
		of << "\t\t{" << endl;
		of << "\t\t\t\"texture\" : " << "\"" << ConvertRS2S(terrain.layer[ i].texture) << "\"" << endl;
		of << "\t\t}"; 
		if (i < 3)
			of << ",";
		of << endl;
	}
	of << "\t]," << endl;
	

	of << "\t\"model\" : [" << endl;
	for (u_int i=0; i < terrain.models.size(); ++i)
	{
		of << "\t\t{" << endl;
		of << "\t\t\t\"filename\" : " << "\"" << ConvertRS2S(terrain.models[ i].fileName)
			<< "\"," << endl;
		of << "\t\t\t\"tm1\" : " << "\"" << 
			terrain.models[ i].tm._11 << " " << terrain.models[ i].tm._12 << " " << 
			terrain.models[ i].tm._13 << " " << terrain.models[ i].tm._14 << "\"," << endl;
		of << "\t\t\t\"tm2\" : " << "\"" << 
			terrain.models[ i].tm._21 << " " << terrain.models[ i].tm._22 << " " << 
			terrain.models[ i].tm._23 << " " << terrain.models[ i].tm._24 << "\"," << endl;
		of << "\t\t\t\"tm3\" : " << "\"" << 
			terrain.models[ i].tm._31 << " " << terrain.models[ i].tm._32 << " " << 
			terrain.models[ i].tm._33 << " " << terrain.models[ i].tm._34 << "\"," << endl;
		of << "\t\t\t\"tm4\" : " << "\"" << 
			terrain.models[ i].tm._41 << " " << terrain.models[ i].tm._42 << " " << 
			terrain.models[ i].tm._43 << " " << terrain.models[ i].tm._44 << "\"" << endl;

		of << "\t\t}"; 
		if (i <  terrain.models.size()-1)
			of << ",";
		of << endl;
	}
	of << "\t]" << endl;

	of << "\t}" << endl;
	of << "}" << endl;


	return true;
}

