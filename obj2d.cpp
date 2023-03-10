#include "all.h"

// 空きを探してOBJ2Dを生成
OBJ2D* OBJ2D::searchSet(OBJ2D* obj, int type, VECTOR2 pos, bool hostilityFg)
{
    if (obj->exist_) return nullptr;

    obj->type_ = type;
    obj->transform_->setPos(pos);
    obj->transform_->setOrgPos(obj->transform_->pos());
    obj->exist_ = true;
    obj->hostilityFg_ = hostilityFg;

    return obj;
}


// 配列ver
OBJ2D* OBJ2D::searchSet(OBJ2D* arr[], int arrayNum, int type, VECTOR2 pos,  bool hostilityFg)
{
    for (int i = 0; i < arrayNum; ++i)
    {
        if (arr[i]->exist_) continue;

        arr[i]->type_  = type;
        arr[i]->transform_->setPos(pos);
        arr[i]->exist_ = true;
        arr[i]->hostilityFg_ = hostilityFg;

        return arr[i];
    }
    return nullptr;
}

OBJ2D* OBJ2D::searchSet(OBJ2D* arr[], int arrayNum, int type, int sprNo,VECTOR2 pos, VECTOR2 scale,  bool hostilityFg)
{
    for (int i = 0; i < arrayNum; ++i)
    {
        if (arr[i]->exist_) continue;

        arr[i]->type_  = type;
        arr[i]->renderer_->setSprNo(sprNo);
        arr[i]->transform_->setPos(pos);
        arr[i]->transform_->setScale(scale);
        arr[i]->hostilityFg_ = hostilityFg;
        arr[i]->exist_ = true;

        return arr[i];
    }
    return nullptr;
}

// OBJ2Dのエリア外チェック
bool OBJ2D::areaCheck(OBJ2D* obj, float limit)
{
    if (obj->transform_->pos().x < -limit ||
        obj->transform_->pos().x > BG::WINDOW_W + limit ||
        obj->transform_->pos().y < -limit ||
        obj->transform_->pos().y > BG::WINDOW_H + limit)
    {
        obj->exist_ = false;
    }

    return obj->exist_;
}



