#include "../../GameLib/game_lib.h"
#include <Shlwapi.h>

#include "textureData.h"
#include "filer.h"

#include "ManageMemory.h"
				   
#pragma comment ( lib, "Shlwapi.lib" )


////////////////////////////
/// 値初期化
//////////////////////////
TextureData::TextureData():
	tex2D(NULL),
	resourceView(NULL)
{
	ZeroMemory(name, sizeof(name) );
}
/////////////////////////////
/// リソース開放
//////////////////////////
TextureData::~TextureData()
{
	SAFE_RELEASE(tex2D);
	SAFE_RELEASE(resourceView);
	SAFE_RELEASE(samplerState);
}
/////////////////////////////////////////////
/// テクスチャ読み込み
/// マネージャからのみアクセス可能
/// manageTextureFile.cpp
//////////////////////////////////////////////
void TextureData::Read(const TCHAR* fileName)
{
	StrCpy( name, fileName );
	ID3D11Resource* pRes;

	// メモリから読み込み
	Filer file;
#if _UNICODE
	char cFileName[256];
	WideCharToMultiByte( CP_ACP, 0, fileName, -1, cFileName, sizeof(cFileName), 0, 0 );
	file.FileOpen(cFileName);
#else
	file.FileOpen(fileName);
#endif
									   
	// 領域確保
	TCHAR* data = NEW TCHAR[file.GetSize()];
	file.AllRead(data);
	HRESULT hr = D3DX11CreateTextureFromMemory(
		GameLib::DirectX11::getDevice(), 
		data,
		file.GetSize(),
		NULL, 
		NULL,
		&pRes,
		NULL );

	SAFE_DELETE(data);
	// ファイルから
	//HRESULT hr = D3DX11CreateTextureFromFile(
	//	GetDevice(),
	//	fileName,
	//	NULL, 
	//	NULL,
	//	&pRes,
	//	NULL );
	//ASSERT_HR( "テクスチャ読み込みエラー", hr );
	tex2D = static_cast<ID3D11Texture2D*>(pRes);

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc( &texDesc );

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceViewを作成する
	hr = GameLib::DirectX11::getDevice()->CreateShaderResourceView( tex2D, &srvDesc, &resourceView );
 
	// シェーダ用にサンプラを作成する
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = GameLib::DirectX11::getDevice()->CreateSamplerState( &samDesc, &samplerState );
}
///////////////////////////////////////////////////////////////////
/// テクスチャを自前で作った場合に使う
/// 名前とデータを与えてマネージャで使えるようにする
/// ついでにサンプラも
/////////////////////////////////////////////////////////////////
void TextureData::SetData(const TCHAR* name, ID3D11Texture2D* tex2D)
{
	HRESULT hr;

	StrCpy( this->name, name );
	this->tex2D = tex2D;

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc( &texDesc );

	// ShaderResourceViewの情報を作成する
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceViewを作成する
	hr = GameLib::DirectX11::getDevice()->CreateShaderResourceView( tex2D, &srvDesc, &resourceView );
 
	// シェーダ用にサンプラを作成する
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = GameLib::DirectX11::getDevice()->CreateSamplerState( &samDesc, &samplerState );
}