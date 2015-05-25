#pragma once


namespace graphic
{

	class cTerrainEditor : public cTerrain
	{
	public:
		cTerrainEditor();
		virtual ~cTerrainEditor();

		bool CreateFromGRDFile( const string &fileName );
		bool WriteGRDFile( const string &fileName );
		bool WriteTRNFile( const string &fileName );
		bool WriteTerrainTextureToPNGFile( const string &fileName );
		void GenerateRawTerrain( OUT sRawTerrain &out );

		void BrushTerrain( const cTerrainCursor &cursor, const float elapseT );
		void BrushTexture( const cTerrainCursor &cursor );

		void SetHeightFactor(const float heightFactor);
		void SetTextureUVFactor(const float textureUVFactor);


	protected:
		void GetTextureUV(const Vector3 &pos, OUT float &tu, OUT float &tv);

	};

}
