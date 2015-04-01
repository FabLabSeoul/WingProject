#pragma once

namespace graphic 
{
	struct sRawMesh;

	namespace importer 
	{

		bool ReadRawMeshFile( const string &fileName, OUT sRawMeshGroup &out );

		bool ReadRawAnimationFile( const string &fileName, OUT sRawAniGroup &out );

		RESOURCE_TYPE::TYPE GetFileKind( const string &fileName );

	}
}

