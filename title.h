#pragma once

class Title : public Scene
{
private:
    static Title instance_;

    struct TitleObj {
        VECTOR2 pos{};
        VECTOR2 velocity{};
        VECTOR2 hitSize{};
        VECTOR2 offset{};
        VECTOR2 scale{};
    };

public:
    static int skyColorNum_;

    static TitleObj titleRabbitNormal_;
    static TitleObj titleStart_;
    static TitleObj titleRule_;
    static TitleObj titleEnd_;

    static int changeSceneNum_;

private:
    static float titleCloudAlpha_;
    static float titleCloudAddAlpha_;

    static TitleObj titleRabbitFire_;
    static TitleObj titleRabbitWind_;

    static TitleObj titleLogo_;

public:
    static Title* instance() { return &instance_; }


    void init() override;
    void deinit() override;
    void update() override;
    void draw() override;

private:
    Title() {}
    Title(const Title&) = delete; // コピーコンストラクタ がないのを明示
};