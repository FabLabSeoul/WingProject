#pragma once


namespace common
{

	string GetFilePathExceptFileName(const std::string &fileName);
	string GetFileExt(const string &fileName);
	string GetFullFileName(const string &fileName);
	string GetFileName(const string &fileName);
	string GetFileNameExceptExt(const string &fileName);
	string RelativePathTo(const string &pathFrom, const string &pathTo);
	bool IsRelativePath(const string &path);
	

	bool CollectFiles( const list<string> &findExt, const string &searchPath, OUT list<string> &out );
	bool FindFile( const string &findName, const string &searchPath, string &out  );

}
