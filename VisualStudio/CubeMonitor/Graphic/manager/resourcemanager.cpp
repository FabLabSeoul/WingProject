
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

	// �޽� �̸� ���� fileName + meshName
	// ���͸��� ����.
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


// �ִϸ��̼� ���� �ε�.
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


// meshName�� �ش��ϴ� �޽����۸� �����Ѵ�.
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


// meshName���� �޽����۸� ã�� �����Ѵ�.
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
	//{ // ���ε��� �����̶�� �����ϰ� ���� �� ó�� ó���Ѵ�.
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
	//{ // ���ε��� �����̶�� �����ϰ� ���� �� ó�� ó���Ѵ�.
	//	delete it->second;
	//	m_meshes.erase(fileName);
	//	m_reLoadFile.erase(fileName);
	//	return NULL;
	//}

	return it->second;
}


// �ؽ��� �ε�.
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

// �ؽ��� �ε�.
// fileName �� �ش��ϴ� ������ ���ٸ�, "./media/" + dirPath  ��ο��� ������ ã�´�.
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


// ���̴� �ε�.
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
				return NULL; // ���� ����.
			}
		}
		else
		{
			string msg = fileName + " ������ �������� �ʽ��ϴ�.";
			MessageBoxA( NULL, msg.c_str(), "ERROR", MB_OK);
		}
	}

	m_shaders[ fileName] = shader;
	return shader;
}


// �ؽ��� ã��.
cTexture* cResourceManager::FindTexture( const string &fileName )
{
	auto it = m_textures.find(fileName);
	if (m_textures.end() == it)
		return NULL; // not exist
	return it->second;
}


// ���̴� ã��.
cShader* cResourceManager::FindShader( const string &fileName )
{
	auto it = m_shaders.find(fileName);
	if (m_shaders.end() == it)
		return NULL; // not exist
	return it->second;
}


// media �������� fileName �� �ش��ϴ� ������ �����Ѵٸ�,
// ��ü ��θ� �����Ѵ�. ������ ã�� ���� �����̸��� ���Ѵ�.
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


// ���� ������ �����Ѵ�.
RESOURCE_TYPE::TYPE cResourceManager::GetFileKind( const string &fileName )
{
	return importer::GetFileKind(fileName);
}


// ���ϰ�� fileName�� media ������ ��� �ּҷ� �ٲ㼭 �����Ѵ�.
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


// �̹� �ε��� ������ �������� �ʰ�, �ٽ� �ε��Ѵ�.
// �޽�, �ִϸ��̼Ǹ� �ش�ȴ�.
void cResourceManager::ReloadFile()
{
	// �ϴ� ��� ��.
	// �� ����� ����� �����Ϸ���, ��ü ������ �ʿ��ϴ�.
	// ������ �޸𸮸� �����ϴ� ��찡 �߻��Ѵ�.

	//BOOST_FOREACH (auto kv, m_meshes)
	//	m_reLoadFile.insert(kv.first);
	//BOOST_FOREACH (auto kv, m_anies)
	//	m_reLoadFile.insert(kv.first);
}
