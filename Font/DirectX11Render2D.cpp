#include "../../GameLib/game_lib.h"

#include "directX11render2D.h"
//#include "directX11Device.h"
//#include "myWindows.h"
#include "ManageMemory.h"

#include <Shlwapi.h>
#include <tchar.h>	
//#include "debug.h"
				   
/////////////////////////////////////////////////
/// @param �o�b�t�@�쐬
/// @brief desc : �\��
/// @brief data : ���_���Ȃ� �������f�[�^ �V���O���|�C���^
/// @brief buffer : �o�b�t�@
/////////////////////////////////////////////
template <class T>
void CreateBuffer(D3D11_BUFFER_DESC* desc, T data, ID3D11Buffer** buffer)
{
	HRESULT hr;
    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(vertexData));
    //vertexData.pSysMem = reinterpret_cast<void*>(data);
    vertexData.pSysMem = (data);

	hr = GameLib::DirectX11::getDevice()->CreateBuffer(desc, &vertexData, buffer);
}					
/////////////////////////////
/// �V�F�[�_�̃R���p�C��
//////////////////////////
HRESULT CompileShaderFromFile( 
	const TCHAR* filename, 
	const char* entryPoint,
	const char* shaderModel,
	ID3DBlob** ppBlobOut )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	do{ // �R���p�C������������܂œǂݍ���
		hr = D3DX11CompileFromFile(
			filename,		// �t�@�C����
			NULL,			// �}�N��
			NULL,			// �C���N���[�h�t�@�C��
			entryPoint,		// �G���g���|�C���g�֐���
			shaderModel,	// �V�F�[�_���f��
			dwShaderFlags,	// �V�F�[�_�R���p�C���t���O
			0,				// �G�t�F�N�g�R���p�C���t���O
			NULL,			// �X���b�h�|���v�C���^�[�t�F�[�X�ւ̃|�C���^
			ppBlobOut, 
			&pErrorBlob, 
			NULL );
		/////////////////////////////////////////////////
		// �ǂݍ��݃G���[
		if( FAILED(hr) )
		{
			if( pErrorBlob != NULL )
			{
			}
			else
			{
			}
		}
		if (pErrorBlob)
		{
			SAFE_RELEASE(pErrorBlob);
		}
	} while ( FAILED(hr) );

	return hr;
}


///---------------------------------
/// 2D�p�ɕϊ�
///---------------------------------
XMVECTOR Trans2D(float x, float y)
{
	return XMVectorSet( 
		static_cast<float>((x-GameLib::window::getWidth()*0.5f)/(GameLib::window::getWidth()*0.5f)),
		static_cast<float>((y-GameLib::window::getHeight()*0.5f)/-(GameLib::window::getHeight()*0.5f)),
		0.0f,
		1.0f
		);
}
///---------------------------------
/// 2D�p�ɕϊ�
///---------------------------------
XMVECTOR Trans2D(int x, int y)
{
	return XMVectorSet( 
		static_cast<float>((x- GameLib::window::getWidth() * 0.5f)/(GameLib::window::getWidth() * 0.5f)),
		static_cast<float>((y- GameLib::window::getHeight() * 0.5f)/-(GameLib::window::getHeight() * 0.5f)),
		0.0f,
		1.0f
		);
}

///---------------------------------
/// 2D�p�ɕϊ�
///---------------------------------
XMVECTOR Trans2D(XMVECTOR vec)
{
	return XMVectorSet( 
		static_cast<float>((XMVectorGetX(vec)- GameLib::window::getWidth() * 0.5f)/(GameLib::window::getWidth() * 0.5f)),
		static_cast<float>((XMVectorGetY(vec)- GameLib::window::getHeight() * 0.5f)/-(GameLib::window::getHeight() * 0.5f)),
		0.0f,
		1.0f
		);
}


XMVECTOR Trans2D(float x, float y);	/// 2D�p�ɕϊ�
XMVECTOR Trans2D(int x, int y);	/// 2D�p�ɕϊ�
XMVECTOR Trans2D(XMVECTOR vec);	/// 2D�p�ɕϊ�

XMVECTOR SetXY(float x, float y); ///< x��y�����̂ݕύX

Render2D::Render2D() : 
	vertexBuffer(NULL),
	indexBuffer(NULL),
	constantBuffer(NULL),
	inputLayout(NULL),
	vertexShader(NULL),
	pixelShader(NULL)
{
}
Render2D::~Render2D()
{
}
void Render2D::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();

	CreateShader();
}
//////////////////////////////////////////////
/// �V�F�[�_�̍쐬
////////////////////////////////////
void Render2D::CreateShader()
{
	HRESULT hr;
	// vertex shader
	ID3DBlob* vblob = NULL;
	hr = CompileShaderFromFile(TEXT("2d.fx"), "vsMain", "vs_4_0_level_9_1", &vblob);
	hr = GameLib::DirectX11::getDevice()->CreateVertexShader( vblob->GetBufferPointer(), vblob->GetBufferSize(), NULL, &vertexShader);
	// Create InputLayout VS�݂̂ɕK�v�@�ǂ̂悤�ȗv�f������
	hr = GameLib::DirectX11::getDevice()->CreateInputLayout(
		vbElement_Pos_Col_Tex, 
		sizeof(vbElement_Pos_Col_Tex)/sizeof(D3D11_INPUT_ELEMENT_DESC),	// ���̓f�[�^��
		vblob->GetBufferPointer(),
		vblob->GetBufferSize(),
		&inputLayout);

	// pixel shader
	ID3DBlob* pblob = NULL;
	hr = CompileShaderFromFile(TEXT("2d.fx"), "psMain", "ps_4_0_level_9_1", &pblob);
	hr = GameLib::DirectX11::getDevice()->CreatePixelShader( pblob->GetBufferPointer(), pblob->GetBufferSize(), NULL, &pixelShader);


	nowVertexShader = vertexShader;
	nowPixelShader = pixelShader;
	
}
//////////////////////////////////////////////
/// �o�[�e�b�N�X�o�b�t�@�̍쐬
////////////////////////////////////////////
void Render2D::CreateVertexBuffer()
{
	// Create VB
	vertices[0].uv = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].uv = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].uv = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f);
	vertices[3].uv = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f);

	vertices[0].color = XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].color = XMVectorSet( 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].color = XMVectorSet( 1.0f, 1.0f, 0.0f, 1.0f);
	vertices[3].color = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f);

	unsigned int vsize = sizeof(vertices);

	D3D11_BUFFER_DESC vdesc;
	ZeroMemory(&vdesc, sizeof(vdesc));
	vdesc.ByteWidth      = vsize;
	vdesc.Usage          = D3D11_USAGE_DEFAULT;
	vdesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vdesc.CPUAccessFlags = 0;


	CreateBuffer(&vdesc, &vertices, &vertexBuffer);

}

//////////////////////////////////////////////
/// �C���f�b�N�X�o�b�t�@�̍쐬
////////////////////////////////////////////
void Render2D::CreateIndexBuffer()
{
	// Create IB
	unsigned int pIndices[] = 
	{
		1,2,0,3
	};
	unsigned int isize = sizeof(pIndices);
	
	D3D11_BUFFER_DESC idesc;
	ZeroMemory(&idesc, sizeof(idesc));
	idesc.ByteWidth      = isize;
	idesc.Usage          = D3D11_USAGE_DYNAMIC;
	idesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	idesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CreateBuffer(&idesc, &pIndices, &indexBuffer);
	

}
//////////////////////////////////////////////
///		�萔�o�b�t�@�̍쐬
////////////////////////////////////////////
void Render2D::CreateConstantBuffer()
{
	
	XMMATRIX offset;
	
	offset = XMMatrixTranslation( 0.0f, 0.0, 0);

	//////////////////////////////////////////////////////
	// �萔�o�b�t�@���쐬����
	//////////////////////////////////////////////////////
	unsigned int nSize = sizeof(offset);
	
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = 0;
	hBufferDesc.ByteWidth = nSize;
	CreateBuffer( &hBufferDesc, &offset, &constantBuffer );


}
///////////////////////////////////
/// �摜����
///////////////////////////////
void Render2D::Render(
		float x1, float y1, 
		float x2, float y2,
		TextureData* texture, 
		VECTOR4 color)
{
	vertices[0].uv = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1].uv = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].uv = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f);
	vertices[3].uv = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[0].pos = Trans2D(x1, y1);
	vertices[1].pos = Trans2D(x2, y1);
	vertices[2].pos = Trans2D(x2, y2);
	vertices[3].pos = Trans2D(x1, y2);

	//
	vertices[0].color = { color.x, color.y, color.z, color.w};
	vertices[1].color = { color.x, color.y, color.z, color.w};
	vertices[2].color = { color.x, color.y, color.z, color.w};
	vertices[3].color = { color.x, color.y, color.z, color.w};

	GameLib::DirectX11::getContext()->UpdateSubresource( vertexBuffer, 0, NULL, vertices, 0, 0 );

	nowPixelShader = pixelShader;

	
	// VB�̃Z�b�g
	ID3D11Buffer* pBufferTbl[] = { vertexBuffer };
	UINT SizeTbl[] = { sizeof(Vertex_Pos_Col_Tex) };
	UINT OffsetTbl[] = { 0 };
	GameLib::DirectX11::getContext()->IASetVertexBuffers(0, 1, pBufferTbl, SizeTbl, OffsetTbl);
	// IB�̃Z�b�g
	GameLib::DirectX11::getContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// IL�̃Z�b�g
	GameLib::DirectX11::getContext()->IASetInputLayout(inputLayout);
	// �v���~�e�B�u�^�C�v�̃Z�b�g
	//SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Shader�̃Z�b�g�A�b�v
	GameLib::DirectX11::getContext()->VSSetShader(nowVertexShader, NULL, 0);
	GameLib::DirectX11::getContext()->VSSetConstantBuffers( 0, 1, &constantBuffer );
	GameLib::DirectX11::getContext()->PSSetShader(nowPixelShader, NULL, 0);
	
	// �����_�����O��ݒ�
	//DirectX11Device::GetInstance()->SetRenderTarget();

	// �e�N�X�`���ݒ�
	GameLib::DirectX11::getContext()->PSSetShaderResources( 0, 1, texture->GetResourceView() );
	GameLib::DirectX11::getContext()->PSSetSamplers( 0, 1, texture->GetSamplerState() );

	// �`��
	GameLib::DirectX11::getContext()->DrawIndexed(4, 0, 0);
}