#include <sys/stat.h>

#include <iostream>
#include "filer.h"

/////////////////////////////////
/// 0������
//////////////////////////////
Filer::Filer() : 
	fp(0),
	size(0)
{
}
/////////////////////////////////
/// �t�@�C�����J���Ă��������
//////////////////////////////////
Filer::~Filer()
{
	if( fp )
	{
		fclose(fp);
	}
}
///////////////////////////////////
/// �t�@�C�����J��
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
	// �T�C�Y�擾
	struct stat statBuf;
	stat( filePath, &statBuf );
	size = statBuf.st_size;
}
///////////////////////////////////
/// �t�@�C�����J��
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