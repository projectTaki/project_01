#pragma once
#include <crtdbg.h>

// インターフェイス解放
#define SAFE_RELEASE(x)  if(x){x->Release();x=NULL;}
#define SAFE_DELETE(x)  if(x){delete x;x=NULL;}
#define SAFE_DELETE_ARRAY(x)  if(x){delete[] x;x=NULL;}
#define SAFE_DELOBJ(p)  { if (p) { DeleteObject(p); (p)=NULL; } }


#define _CRTDBG_MAP_ALLOC
#if defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC) && !defined(NEW)
#define NEW  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW  new
#endif