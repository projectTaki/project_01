#pragma once

class Over : public Scene
{
private:
    static Over instance_;

    // pos, velocity, hitSize, offset, scale
    struct OverObj {
        VECTOR2 pos{};
        VECTOR2 velocity{};
        VECTOR2 hitSize{};
        VECTOR2 offset{};
        VECTOR2 scale{};
    };

private:
    static OverObj overLogo_;

public:
    static OverObj overTitle_;
    static OverObj overRetry_;

    static bool isRetry_;

public:
    static Over* instance() { return &instance_; }

    void init() override;
    void deinit() override;
    void update() override;
    void draw() override;

private:
    Over() {}
    Over(const Over&) = delete; // コピーコンストラクタ がないのを明示
};