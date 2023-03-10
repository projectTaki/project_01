#pragma once
#include <d3dx11.h>
#include <map>
#include <string>
#include "textureData.h"



////////////////////////////////////
/// �����e�N�X�`�����쐬����N���X
////////////////////////////////////
class FontTexture
{
	TextureData* textureData;
public:	   
	FontTexture();
	~FontTexture();
	TextureData* GetTexture();	///< �����e�N�X�`�����擾
	TextureData* Create(TCHAR* c, int fontSize);				///< �����e�N�X�`���̍쐬
};


