#include "all.h"
#include "Font\directX11Render2D.h"
#include "Font\fontTexture.h"

#pragma comment(lib,"d3d11.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif

#pragma comment(lib, "legacy_stdio_definitions.lib")

int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE,_In_ LPWSTR, _In_ int)
{
	SceneManager sceneManager;
	sceneManager.execute(Title::instance());

	return 0;
}