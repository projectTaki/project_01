
#pragma once 


#include <d3dx11.h>

////////////////////////////////////////////////////
/// テクスチャ情報クラス
/// テクスチャに関する情報を保持する
////////////////////////////////////////////////////
class TextureData
{
	// マネージャで読み込むときのみ使う
	friend TextureData* ReadTextureData(const TCHAR* fileName);
private:
	void Read(const TCHAR* fileName); ///< 使いたいときはフレンド

protected:

	TCHAR name[256];
	ID3D11Texture2D* tex2D;					///< 2Ｄテクスチャ
	ID3D11ShaderResourceView* resourceView; ///<シェーダとリソースを繋ぐインターフェイス
	ID3D11SamplerState* samplerState;

public:
	TextureData();
	~TextureData();
public: // アクセサ

	void SetData(const TCHAR* name, ID3D11Texture2D* tex2D);

	const TCHAR* GetName() const { return name; }
	ID3D11ShaderResourceView** GetResourceView() { return &resourceView; }
	ID3D11SamplerState** GetSamplerState() { return &samplerState; }
	ID3D11Texture2D* GetTexture() { return tex2D; }
};