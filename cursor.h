#pragma once

class Cursor
{
private:
    int state_;
    int timer_;
    POINT csPoint_;

public:
    static OBJ2D* cursor_;

public:
    Cursor()
        :state_(0)
        , timer_(0)
        , csPoint_()
    {}
    ~Cursor() { safe_delete(cursor_); }

    void init();
    void deinit();
    void update();
    void render();
    void setCursor();   // カーソルのパラメータ設定

public:
    OBJ2D* cursor() const { return cursor_; }
};