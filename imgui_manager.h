#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class ImGuiManager
{
public:
    static ImGuiManager& getInstance()
    {
        static ImGuiManager instance;
        return instance;
    };

    void init();
    void shutdown();
    void newFrame();
    void render();
};
