#pragma once

class Clear : public Scene
{
public:
    static const int clearCarpetMax = 23;

private:
    static Clear instance_;

    // pos, velocity, hitSize, offset, scale, sprNo, anime, animeTimer
    struct ClearObj {
        VECTOR2 pos{};
        VECTOR2 velocity{};
        VECTOR2 hitSize{};
        VECTOR2 offset{};
        VECTOR2 scale{};
    };

    struct RabbitObj {
        int     sprNo = 0;
        VECTOR2 pos{};
        VECTOR2 anime{};
        int     animeTimer = 0;
        int     animeStartTime = 0;
    };

private:
    static ClearObj clearLogo_;

    static RabbitObj clearCarpet_[clearCarpetMax];
    static RabbitObj clearRabbit_;

public:
    static ClearObj clearTitle_;
    static ClearObj clearRetry_;

    static bool isRetry_;

public:
    static Clear* instance() { return &instance_; }

    void init() override;
    void deinit() override;
    void update() override;
    void draw() override;

private:
    Clear() {}
    Clear(const Clear&) = delete; // コピーコンストラクタ がないのを明示
};

