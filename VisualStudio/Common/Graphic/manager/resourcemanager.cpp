
#include "stdafx.h"
#include "resourcemanager.h"
#include "../importer/modelimporter.h"
#include "../base/material.h"


using namespace graphic;


cResourceManager::cResourceManager() :
	m_mediaDirectory("./media/")
{

}

cResourceManager::~cResourceManager()
{
	Clear();
}


// load model file
sRawMeshGroup* cResourceManager::LoadModel( const string &fileName )
{
	RETV(fileName.empty(), NULL);

	if (sRawMeshGroup *data = FindModel(fileName))
		return data;

	sRawMeshGroup *meshes = new sRawMeshGroup;
	meshes->name = fileName;

	if (!importer::ReadRawMeshFile(fileName, *meshes))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!importer::ReadRawMeshFile(newPath, *meshes))
			{
				goto error;
			}
		}
		else
		{
			goto error;
		}
	}

	// 메쉬 이름 설정 fileName + meshName
	// 메터리얼 설정.
	BOOST_FOREACH (auto &mesh, meshes->meshes)
	{
		mesh.name = fileName + "::" + mesh.name;
		if (mesh.mtrlId >= 0)
		{
			mesh.mtrl = meshes->mtrls[ mesh.mtrlId];
		}
	}

	m_meshes[ fileName] = meshes;
	return meshes;

error:
	delete meshes;
	return NULL;
}


// 애니메이션 파일 로딩.
sRawAniGroup* cResourceManager::LoadAnimation( const string &fileName )
{
	RETV(fileName.empty(), NULL);

	if (sRawAniGroup *data = FindAnimation(fileName))
		return data;

	sRawAniGroup *anies = new sRawAniGroup;
	anies->name = fileName;

	if (!importer::ReadRawAnimationFile(fileName, *anies))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!importer::ReadRawAnimationFile(newPath, *anies))
			{
				goto error;
			}
		}
		else
		{
			goto error;
		}
	}

	m_anies[ fileName] = anies;
	return anies;

error:
	delete anies;
	return NULL;
}


// meshName에 해당하는 메쉬버퍼를 리턴한다.
cMeshBuffer* cResourceManager::LoadMeshBuffer( const string &meshName)
{
	if (cMeshBuffer *data = FindMeshBuffer(meshName))
		return data;

	string fileName = meshName;
	fileName.erase(meshName.find("::"));

	if (sRawMeshGroup *meshes = LoadModel(fileName))
	{
		BOOST_FOREACH (auto &rawMesh, meshes->meshes)
		{
			if (meshName == rawMesh.name)
			{
				cMeshBuffer *buffer = new cMeshBuffer(rawMesh);
				m_mesheBuffers[ meshName] = buffer;
				return buffer;
			}
		}
	}
	
	return NULL;
}


// meshName으로 메쉬버퍼를 찾아 리턴한다.
cMeshBuffer* cResourceManager::FindMeshBuffer( const string &meshName )
{
	auto it = m_mesheBuffers.find(meshName);
	if (m_mesheBuffers.end() == it)
		return NULL; // not exist

	return it->second;
}


// find model data
sRawMeshGroup* cResourceManager::FindModel( const string &fileName )
{
	auto it = m_meshes.find(fileName);
	if (m_meshes.end() == it)
		return NULL; // not exist

	//if (m_reLoadFile.end() != m_reLoadFile.find(fileName))
	//{ // 리로드할 파일이라면 제거하고 없는 것 처럼 처리한다.
	//	delete it->second;
	//	m_meshes.erase(fileName);
	//	m_reLoadFile.erase(fileName);
	//	return NULL;
	//}

	return it->second;
}


// find animation data
sRawAniGroup* cResourceManager::FindAnimation( const string &fileName )
{
	auto it = m_anies.find(fileName);
	if (m_anies.end() == it)
		return NULL; // not exist

	//if (m_reLoadFile.end() != m_reLoadFile.find(fileName))
	//{ // 리로드할 파일이라면 제거하고 없는 것 처럼 처리한다.
	//	delete it->second;
	//	m_meshes.erase(fileName);
	//	m_reLoadFile.erase(fileName);
	//	return NULL;
	//}

	return it->second;
}


// 텍스쳐 로딩.
cTexture* cResourceManager::LoadTexture( const string &fileName, const bool isSizePow2 )//isSizePow2=true
{
	if (cTexture *p = FindTexture(fileName))
		return p;

	cTexture *texture = new cTexture();
	if (!texture->Create(fileName, isSizePow2))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!texture->Create(newPath, isSizePow2))
			{
				delete texture;
				return false;
			}
		}
	}
	m_textures[ fileName] = texture;
	return texture;
}

// 텍스쳐 로딩.
// fileName 에 해당하는 파일이 없다면, "./media/" + dirPath  경로에서 파일을 찾는다.
cTexture* cResourceManager::LoadTexture( const string &dirPath, const string &fileName, const bool isSizePow2)
	//isSizePow2=true
{
	if (cTexture *p = FindTexture(fileName))
		return p;

	cTexture *texture = new cTexture();
	if (!texture->Create(fileName, isSizePow2))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory +dirPath+"/", newPath))
		{
			if (!texture->Create(newPath, isSizePow2))
			{
				delete texture;
				return false;
			}
		}
	}
	m_textures[ fileName] = texture;
	return texture;
}


// 셰이더 로딩.
cShader* cResourceManager::LoadShader( const string &fileName )
{
	if (cShader *p = FindShader(fileName))
		return p;

	cShader *shader = new cShader();
	if (!shader->Create(fileName, "TShader", false))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!shader->Create(newPath, "TShader"))
			{
				delete shader;
				return NULL; // 실패 종료.
			}
		}
		else
		{
			string msg = fileName + " 파일이 존재하지 않습니다.";
			MessageBoxA( NULL, msg.c_str(), "ERROR", MB_OK);
		}
	}

	m_shaders[ fileName] = shader;
	return shader;
}


// 텍스쳐 찾기.
cTexture* cResourceManager::FindTexture( const string &fileName )
{
	auto it = m_textures.find(fileName);
	if (m_textures.end() == it)
		return NULL; // not exist
	return it->second;
}


// 셰이더 찾기.
cShader* cResourceManager::FindShader( const string &fileName )
{
	auto it = m_shaders.find(fileName);
	if (m_shaders.end() == it)
		return NULL; // not exist
	return it->second;
}


// media 폴더내에 fileName 에 해당하는 파일이 존재한다면,
// 전체 경로를 리턴한다. 파일을 찾을 때는 파일이름만 비교한다.
string cResourceManager::FindFile( const string &fileName )
{
	string newPath;
	if (common::FindFile(fileName, m_mediaDirectory, newPath))
	{
		return newPath;
	}

	return ""; //empty string
}


// remove all data
void cResourceManager::Clear()
{
	// remove raw mesh
	BOOST_FOREACH (auto kv, m_meshes)
	{
		delete kv.second;
	}
	m_meshes.clear();

	// remove texture
	BOOST_FOREACH (auto kv, m_textures)
	{
		delete kv.second;
	}
	m_textures.clear();

	// remove raw ani
	BOOST_FOREACH (auto kv, m_anies)
	{
		delete kv.second;
	}
	m_anies.clear();

	// remove shader
	BOOST_FOREACH (auto kv, m_shaders)
	{
		delete kv.second;
	}
	m_shaders.clear();

	// remove mesh buffer
	BOOST_FOREACH (auto kv, m_mesheBuffers)
	{
		delete kv.second;
	}
	m_mesheBuffers.clear();
}


// 파일 종류를 리턴한다.
RESOURCE_TYPE::TYPE cResourceManager::GetFileKind( const string &fileName )
{
	return importer::GetFileKind(fileName);
}


// 파일경로 fileName을 media 폴더의 상대 주소로 바꿔서 리턴한다.
// ex)
// media : c:/project/media,  
// fileName : c:/project/media/terrain/file.txt
// result = ./terrain/file.txt
string cResourceManager::GetRelativePathToMedia( const string &fileName )
{
	const string mediaFullPath = common::GetFullFileName(m_mediaDirectory);
	const string fullFileName = common::GetFullFileName(fileName);
	const string relatePath = common::RelativePathTo( mediaFullPath, fullFileName);
	return relatePath;
}


// 이미 로딩된 파일을 재사용하지 않고, 다시 로드한다.
// 메쉬, 애니메이션만 해당된다.
void cResourceManager::ReloadFile()
{
	// 일단 기능 끔.
	// 이 기능이 제대로 동작하려면, 전체 리셋이 필요하다.
	// 지워지 메모리를 참조하는 경우가 발생한다.

	//BOOST_FOREACH (auto kv, m_meshes)
	//	m_reLoadFile.insert(kv.first);
	//BOOST_FOREACH (auto kv, m_anies)
	//	m_reLoadFile.insert(kv.first);
}
