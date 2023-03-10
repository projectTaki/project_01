#include "dispString.h"

DispString::DispString()
{
    render2D = Render2D::GetInstance();
    render2D->Init();
}

DispString::~DispString()
{
    auto iter = dictionary.begin();
    while (iter != dictionary.end())
    {
        safe_delete(iter->second);
        iter = dictionary.erase(iter);
    }
    dictionary.clear();
}

void DispString::dispString(const TCHAR* str, float x1, float y1, float x2, float y2, float size, VECTOR4 color)
{
    //‰¼
    int num = _tcslen(str);
    for (int i = 0; i < num; ++i)
    {
        if (auto iter = dictionary.find((UINT)((TCHAR*)str[i])); iter != end(dictionary))
        {
            //exist
        }
        else
        {
            //not exists
            FontTexture* font = new FontTexture();
            font->Create((TCHAR*)&str[i], size);
            dictionary.emplace((UINT)((TCHAR*)str[i]), font);
        }
        render2D->Render(x1 + i * x2, y1, x1 + i * x2 + x2, y1 + y2, dictionary[(UINT)str[i]]->GetTexture(), color);
    }
}