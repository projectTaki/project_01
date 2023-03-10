#pragma once

#include "../../GameLib/game_lib.h"
#include <tchar.h>
#include "fontTexture.h"
#include "directX11Render2D.h"

class DispString
{
private:
    Render2D* render2D;
    std::map<UINT, FontTexture*> dictionary;

public:
    DispString();
    ~DispString();

    void dispString(const TCHAR* str, 
        float x1, float y1, float x2, float y2, 
        float size, VECTOR4 color);

    static DispString& instance()
    {
        static DispString instance;
        return instance;
    }
};
