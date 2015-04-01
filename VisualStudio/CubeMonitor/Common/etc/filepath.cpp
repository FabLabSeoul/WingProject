
#include "stdafx.h"
#include "FilePath.h"
#include <shlwapi.h> // �� ��� ���Ͽ� FilePath�� ���õ� �Լ����� ����. �� �̿��غ���.
#pragma comment(lib, "shlwapi")


namespace common {

	bool CompareExtendName(const char *srcFileName, int srcStringMaxLength, const char *compareExtendName);

}


//------------------------------------------------------------------------
// fileName���� �����̸��� Ȯ���ڸ� ������ ������ ��θ� �����Ѵ�.
// �������� '\' ���ڴ� ����.
//------------------------------------------------------------------------
string common::GetFilePathExceptFileName(const std::string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	PathRemoveFileSpecA( srcFileName );
	return srcFileName;
}


/**
 @brief  fileName���� Ȯ���ڸ� �����Ѵ�.
 */
string common::GetFileExt(const string &fileName)
{
	char *ext = PathFindExtensionA(fileName.c_str());
	return ext;
}


// ���丮 ��θ� ������ �� ���� �̸��� �����Ѵ�. (������� �� �̿�).
string common::GetFullFileName(const string &fileName)
{
	char dstFileName[ MAX_PATH] = {NULL, };

	if (IsRelativePath(fileName))
	{
		char curDir[ MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, curDir);
		const string path = string(curDir) + "/" + fileName;
		GetFullPathNameA(path.c_str(), path.size(), dstFileName, NULL);
	}
	else
	{
		//GetFullPathNameA(fileName.c_str(), fileName.size(), dstFileName, NULL);
		return fileName;
	}

	return dstFileName;
}


/**
 @brief fileName�� ���丮 ��θ� ������ �����̸��� Ȯ���ڸ� �����Ѵ�.
 */
string common::GetFileName(const string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	return name;
}


/**
 @brief fileName�� ���丮 ���, Ȯ���ڸ� ������ �����̸��� �����Ѵ�.
 */
string common::GetFileNameExceptExt(const string &fileName)
{
	char srcFileName[ MAX_PATH];
	strcpy_s(srcFileName, MAX_PATH, fileName.c_str() );
	char *name = PathFindFileNameA(srcFileName);
	PathRemoveExtensionA(name);
	return name;
}


// pathFrom��ο��� pathTo ����� ������ �����ϱ� ���� ����θ� �����Ѵ�.
// ex) 
// pathFrom : c:/project/media,  pathTo : c:/project/media/terrain/file.txt
// result = ./terrain/file.txt
string common::RelativePathTo(const string &pathFrom, const string &pathTo)
{
	char szOut[ MAX_PATH];

	PathRelativePathToA(szOut, 
		pathFrom.c_str(), FILE_ATTRIBUTE_DIRECTORY, 
		pathTo.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szOut;
}


// ��� ����̸� true�� �����Ѵ�.
bool common::IsRelativePath(const string &path)
{
	return PathIsRelativeA(path.c_str())? true : false;
}


//-----------------------------------------------------------------------------//
// searchPath������ findExt Ȯ���� ����Ʈ�� ���Ե� ������ out�� �����Ѵ�.
//
// searchPath: Ž���ϰ��� �ϴ� ���丮 ���
// findExt: ã���� �ϴ� Ȯ����, 2���̻� �����Ҽ��ְ� �ϱ����ؼ� ����Ʈ �ڷ����°� �Ǿ���.
// out: ��ġ�ϴ� Ȯ���ڸ� ���� �����̸��� �����Ѵ�.
//-----------------------------------------------------------------------------//
bool common::CollectFiles( const list<string> &findExt, const string &searchPath, OUT list<string> &out )
{
	WIN32_FIND_DATAA fd;
	const string searchDir = searchPath + "*.*";
	HANDLE hFind = FindFirstFileA(searchDir.c_str(), &fd);

	while (1)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ((string(".") != fd.cFileName) && (string("..") != fd.cFileName))
			{
				CollectFiles( findExt, searchPath + string(fd.cFileName) + "/", out ); 
			}
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			const string fileName = fd.cFileName;

			auto it = findExt.begin();
			while (findExt.end() != it)
			{
				if (CompareExtendName(fileName.c_str() , fileName.length(), it->c_str()))
				{
					out.push_back( searchPath + fileName );
					break;
				}
				++it;
			}
		}

		if (!FindNextFileA(hFind, &fd))
			break;
	}

	FindClose(hFind);
	
	return true;
}



//------------------------------------------------------------------------
// srcFileName�� Ȯ���ڿ� compareExtendName �̸��� ���ٸ� true�� �����Ѵ�.
// Ȯ���ڴ� srcFileName ������ '.'�� ���� ������ �̴�.
//------------------------------------------------------------------------
bool common::CompareExtendName(const char *srcFileName, const int srcStringMaxLength, const char *compareExtendName)
{
	const int len = strnlen_s(srcFileName, srcStringMaxLength);
	if (len <= 0)
		return FALSE;

	int count = 0;
	char temp[5];
	for (int i=0; i < len && i < 4; ++i)
	{
		const char c = srcFileName[ len-i-1];
		if ('.' == c)
		{
			break;
		}
		else
		{
			temp[ count++] = c;
		}
	}
	temp[ count] = NULL;

	char extendName[5];
	for (int i=0; i < count; ++i)
		extendName[ i] = temp[ count-i-1];
	extendName[ count] = NULL;

	if (!strcmp(extendName, compareExtendName))
	{
		return true;
	}

	return false;
}


// searchPath ���丮 �ȿ��� findName �� �����̸��� ���� ������ �ִٸ� �ش� ��θ�
// out �� �����ϰ� true �� �����Ѵ�.
bool common::FindFile( const string &findName, const string &searchPath, string &out  )
{
	WIN32_FIND_DATAA fd;
	const string searchDir = searchPath + "*.*";
	HANDLE hFind = FindFirstFileA(searchDir.c_str(), &fd);

	while (1)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ((string(".") != fd.cFileName) && (string("..") != fd.cFileName))
			{
				if (FindFile( findName, searchPath + string(fd.cFileName) + "/", out ))
					break;
			}
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			string fileName = fd.cFileName;
			if (lowerCase(fileName) == lowerCase(GetFileName(findName)))
			{
			//if (fileName == GetFileName(findName))
			//{
				out = searchPath + GetFileName(findName);
				break;
			}
		}

		if (!FindNextFileA(hFind, &fd))
			break;
	}

	FindClose(hFind);

	return !out.empty();
}
