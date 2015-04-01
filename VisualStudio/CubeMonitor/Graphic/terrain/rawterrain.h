#pragma once


namespace graphic
{

	struct sRawTerrrainModel
	{
		string fileName;
		Matrix44 tm;
	};

	struct sRawLayer
	{
		string texture;
	};


	struct sRawTerrain
	{
		int colCellCount;
		int rowCellCount;
		float cellSize;
		int heightMapStyle; // 0=heightMap, 1=grid format
		string heightMap;
		string bgTexture;
		string alphaTexture;
		int alphaTextureWidth;
		int alphaTextureHeight;
		float textureFactor;
		float heightFactor;
		bool renderWater;
		sRawLayer layer[4];
		vector<sRawTerrrainModel> models;
	};


}
