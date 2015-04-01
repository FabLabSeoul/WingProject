#pragma once


namespace graphic 
{ 


	struct sRawTerrain;

	namespace importer 
	{

		bool ReadRawTerrainFile( const string &fileName, OUT sRawTerrain &out );

	}


}