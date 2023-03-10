#pragma once
#include "textureData.h"   
#define _XM_NO_INTRINSICS_
#include <xnamath.h>
				
struct Vertex_Pos_Col_Tex
{
	XMVECTOR pos;
	XMVECTOR color;
	XMVECTOR uv;
};

const D3D11_INPUT_ELEMENT_DESC vbElement_Pos_Col_Tex[] = 
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },

};
class TextureData;

///////////////////////////
/// 矩形描画
/////////////////////////
class Render2D
{
	ID3D11Buffer* vertexBuffer;						///< 頂点バッファ
	ID3D11Buffer* indexBuffer;						///< インデクスバッファ
	ID3D11Buffer* constantBuffer;						///< 定数バッファ
	
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;

	ID3D11VertexShader* nowVertexShader;
	ID3D11PixelShader*  nowPixelShader;

	Vertex_Pos_Col_Tex vertices[4];


	void CreateShader();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateConstantBuffer();
	Render2D();
	~Render2D();
public:
	static Render2D* GetInstance(void)
	{
		static Render2D i;
		return &i;
	}
	void Init();


	void Render(int indexNum = 4);
	/// 画像表示
	void Render(
		float x1, float y1, 
		float x2, float y2,
		TextureData* texture, 
		VECTOR4 color = {1,1,1,1}
		);

};
