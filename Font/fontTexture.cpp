#include "../../GameLib/game_lib.h"
#include <Shlwapi.h>

#include "fontTexture.h"
#include "ManageMemory.h"

///////////////////////////////////////
/// 値初期化
////////////////////////////////////////
FontTexture::FontTexture() :
textureData(0)
{
	DESIGNVECTOR design;
	AddFontResourceEx(
		TEXT("APJapanesefontF.ttf"),
		FR_PRIVATE,
		&design
		);

	  
	textureData = NEW TextureData();
}
///////////////////////////////////////
/// メモリ開放
///////////////////////////////////////
FontTexture::~FontTexture()
{
	DESIGNVECTOR design;
	RemoveFontResourceEx( 
		TEXT("resources/APJapanesefontF.ttf"),
		FR_PRIVATE,
		&design
	);

	// テクスチャデータの削除
	SAFE_DELETE(textureData);
}


#define LOAD_FONT "HG明朝E"

/////////////////////////////////////////////////
/// TODO:フォントテクスチャを作成する
///////////////////////////////////////////////
TextureData* FontTexture::Create( TCHAR* c, int fontsize)
{
	HRESULT hr;
   // フォントの生成
	LOGFONT lf = {
		fontsize,
		0,
		0, 
		0, 
		0, 
		0,
		0,
		0, 
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		TEXT(LOAD_FONT)
	};
	//TEXT("あんずもじ湛")
	//TEXT("HG創英角ﾎﾟｯﾌﾟ体")
	//TEXT("HG行書体")
	
	//TEXT("HG明朝E")
	//TEXT("UD デジタル 教科書体 N-B")
	 
	// ×
	//TEXT("Viner Hand ITC")


	HFONT hFont = CreateFontIndirect( &lf );
	if( !( hFont ) )
	{
	}

	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC( NULL );
	HFONT oldFont = (HFONT)SelectObject( hdc, hFont );

	// 文字コード取得
	UINT code = 0;
	#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
		code = (UINT)*c;
	#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];
	#endif

	// フォントビットマップ取得
	TEXTMETRIC TM;
	GetTextMetrics( hdc, &TM );
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat );
	BYTE* ptr = new BYTE[ size ];
	GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat );

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( NULL, hdc );


	//--------------------------------
	// 書き込み可能テクスチャ作成
	//--------------------------------

	// CPUで書き込みができるテクスチャを作成

	// テクスチャ作成
	D3D11_TEXTURE2D_DESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.Width			= GM.gmCellIncX;
	desc.Height			= TM.tmHeight;
	desc.MipLevels		= 1;
	desc.ArraySize		= 1;
	desc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
	desc.SampleDesc.Count= 1;
	desc.Usage			= D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
	desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
	desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;	// CPUからアクセスして書き込みOK

	ID3D11Texture2D* tex2D;					///< 2Ｄテクスチャ
	hr = GameLib::DirectX11::getDevice()->CreateTexture2D( &desc, 0, &tex2D );
	
	D3D11_MAPPED_SUBRESOURCE hMappedResource{};
	if (tex2D != nullptr)
	{
		hr = GameLib::DirectX11::getContext()->Map(
			tex2D,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&hMappedResource);
		// ここで書き込む
		BYTE* pBits = (BYTE*)hMappedResource.pData;

		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
		int iOfs_x = GM.gmptGlyphOrigin.x;
		int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
		int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
		int iBmp_h = GM.gmBlackBoxY;
		int Level = 17;
		int x, y;
		DWORD Alpha, Color;
		memset(pBits, 0, hMappedResource.RowPitch * TM.tmHeight);
		for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
		{
			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
			{
				Alpha = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
				Color = 0x00ffffff | (Alpha << 24);

				memcpy((BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
			}
		}

		GameLib::DirectX11::getContext()->Unmap(tex2D, 0);
	}
	textureData->SetData( c, tex2D);

	delete[] ptr;
	return textureData;
}
/////////////////////////////////////////////////
/// テクスチャデータを取得する
////////////////////////////////////////////////
TextureData* FontTexture::GetTexture()
{
	return textureData;
}