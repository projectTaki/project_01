#pragma once

#include <stdio.h>

////////////////////////
/// 簡単なファイル読み込み
////////////////////////
class Filer
{
public:
	Filer();
	~Filer();
	void FileOpen(const char* filePath, const char* mode = "rb");
	void FileClose() { fclose(fp); fp = 0; }
	unsigned int GetSize() const { return size; }
	void AllRead(char* buf);
	void AllRead(wchar_t* buf);
private:
	FILE *fp;
	unsigned int size;
};