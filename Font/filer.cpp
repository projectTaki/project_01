#include <sys/stat.h>

#include <iostream>
#include "filer.h"

/////////////////////////////////
/// 0初期化
//////////////////////////////
Filer::Filer() : 
	fp(0),
	size(0)
{
}
/////////////////////////////////
/// ファイルを開いていたら閉じる
//////////////////////////////////
Filer::~Filer()
{
	if( fp )
	{
		fclose(fp);
	}
}
///////////////////////////////////
/// ファイルを開く
/////////////////////////////////
void Filer::FileOpen(const char* filePath, const char* mode)
{
	if( fp )
	{
		fclose(fp);
	}
	errno_t err;
	err = fopen_s( &fp, filePath, mode );
	if( err )
	{ 
	}
	// サイズ取得
	struct stat statBuf;
	stat( filePath, &statBuf );
	size = statBuf.st_size;
}
///////////////////////////////////
/// ファイルを開く
/////////////////////////////////
void Filer::AllRead(char* buf)
{
	unsigned int bufLen = 0;
	bufLen = static_cast<unsigned int>(strlen(buf));
	if( bufLen < size )
	{
	}
	fread( buf, sizeof( char ), bufLen, fp );
	//buf[bufLen] = NULL;
}
void Filer::AllRead(wchar_t* buf)
{
	unsigned int bufLen = 0;
	bufLen = static_cast<unsigned int>(wcslen(buf));
	if( bufLen < size )
	{
	}
	fread( buf, sizeof( char ), bufLen, fp );
	//buf[bufLen] = NULL;
}