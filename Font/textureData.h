
#pragma once 


#include <d3dx11.h>

////////////////////////////////////////////////////
/// �e�N�X�`�����N���X
/// �e�N�X�`���Ɋւ������ێ�����
////////////////////////////////////////////////////
class TextureData
{
	// �}�l�[�W���œǂݍ��ނƂ��̂ݎg��
	friend TextureData* ReadTextureData(const TCHAR* fileName);
private:
	void Read(const TCHAR* fileName); ///< �g�������Ƃ��̓t�����h

protected:

	TCHAR name[256];
	ID3D11Texture2D* tex2D;					///< 2�c�e�N�X�`��
	ID3D11ShaderResourceView* resourceView; ///<�V�F�[�_�ƃ��\�[�X���q���C���^�[�t�F�C�X
	ID3D11SamplerState* samplerState;

public:
	TextureData();
	~TextureData();
public: // �A�N�Z�T

	void SetData(const TCHAR* name, ID3D11Texture2D* tex2D);

	const TCHAR* GetName() const { return name; }
	ID3D11ShaderResourceView** GetResourceView() { return &resourceView; }
	ID3D11SamplerState** GetSamplerState() { return &samplerState; }
	ID3D11Texture2D* GetTexture() { return tex2D; }
};