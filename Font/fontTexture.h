#pragma once
#include <d3dx11.h>
#include <map>
#include <string>
#include "textureData.h"



////////////////////////////////////
/// 文字テクスチャを作成するクラス
////////////////////////////////////
class FontTexture
{
	TextureData* textureData;
public:	   
	FontTexture();
	~FontTexture();
	TextureData* GetTexture();	///< 文字テクスチャを取得
	TextureData* Create(TCHAR* c, int fontSize);				///< 文字テクスチャの作成
};


