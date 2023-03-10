#include "../../GameLib/game_lib.h"
#include <Shlwapi.h>

#include "textureData.h"
#include "filer.h"

#include "ManageMemory.h"
				   
#pragma comment ( lib, "Shlwapi.lib" )


////////////////////////////
/// �l������
//////////////////////////
TextureData::TextureData():
	tex2D(NULL),
	resourceView(NULL)
{
	ZeroMemory(name, sizeof(name) );
}
/////////////////////////////
/// ���\�[�X�J��
//////////////////////////
TextureData::~TextureData()
{
	SAFE_RELEASE(tex2D);
	SAFE_RELEASE(resourceView);
	SAFE_RELEASE(samplerState);
}
/////////////////////////////////////////////
/// �e�N�X�`���ǂݍ���
/// �}�l�[�W������̂݃A�N�Z�X�\
/// manageTextureFile.cpp
//////////////////////////////////////////////
void TextureData::Read(const TCHAR* fileName)
{
	StrCpy( name, fileName );
	ID3D11Resource* pRes;

	// ����������ǂݍ���
	Filer file;
#if _UNICODE
	char cFileName[256];
	WideCharToMultiByte( CP_ACP, 0, fileName, -1, cFileName, sizeof(cFileName), 0, 0 );
	file.FileOpen(cFileName);
#else
	file.FileOpen(fileName);
#endif
									   
	// �̈�m��
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
	// �t�@�C������
	//HRESULT hr = D3DX11CreateTextureFromFile(
	//	GetDevice(),
	//	fileName,
	//	NULL, 
	//	NULL,
	//	&pRes,
	//	NULL );
	//ASSERT_HR( "�e�N�X�`���ǂݍ��݃G���[", hr );
	tex2D = static_cast<ID3D11Texture2D*>(pRes);

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc( &texDesc );

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceView���쐬����
	hr = GameLib::DirectX11::getDevice()->CreateShaderResourceView( tex2D, &srvDesc, &resourceView );
 
	// �V�F�[�_�p�ɃT���v�����쐬����
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
/// �e�N�X�`�������O�ō�����ꍇ�Ɏg��
/// ���O�ƃf�[�^��^���ă}�l�[�W���Ŏg����悤�ɂ���
/// ���łɃT���v����
/////////////////////////////////////////////////////////////////
void TextureData::SetData(const TCHAR* name, ID3D11Texture2D* tex2D)
{
	HRESULT hr;

	StrCpy( this->name, name );
	this->tex2D = tex2D;

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc( &texDesc );

	// ShaderResourceView�̏����쐬����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// ShaderResourceView���쐬����
	hr = GameLib::DirectX11::getDevice()->CreateShaderResourceView( tex2D, &srvDesc, &resourceView );
 
	// �V�F�[�_�p�ɃT���v�����쐬����
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = GameLib::DirectX11::getDevice()->CreateSamplerState( &samDesc, &samplerState );
}