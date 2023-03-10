#pragma once

enum TUTORIAL_STATE
{
    INIT,
    IRIS_IN,
    UPDATE,
    IRIS_OUT_TITLE,
};

class Tutorial : public Scene
{
public:
    static const int pageMax_ = 9 - 1;
private:
    static Tutorial instance_;

    static int ruleSprNo_;
    static int pageNum_;

public:
    static Tutorial* instance() { return &instance_; }

    void init() override;
    void deinit() override;
    void update() override;
    void draw() override;

    void reset();

private:
    Tutorial() {}
    Tutorial(const Tutorial&) = delete; // = delete コピーコンストラクタが存在しないことを明示
};
