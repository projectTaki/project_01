#include "all.h"
#include "imgui_manager.h"

using namespace GameLib;
using namespace input;


bool Judge::rectHitCheck(VECTOR2 pos1, VECTOR2 size1, VECTOR2 pos2, VECTOR2 size2) 
{
    float left1, right1, top1, bottom1;
    float left2, right2, top2, bottom2;

    left1   = pos1.x;
    right1  = pos1.x + size1.x;
    top1    = pos1.y;
    bottom1 = pos1.y + size1.y;
                     
    left2   = pos2.x;
    right2  = pos2.x + size2.x;
    top2    = pos2.y;
    bottom2 = pos2.y + size2.y;

    if (right1 > left2 && left1 < right2 
     && top1 < bottom2 && bottom1 > top2) return true;

    return false;
}


void Judge::titleJudge()
{

    Cursor::cursor_->setIsTouchCmd(false);

    // Start
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Title::titleStart_.pos + Title::titleStart_.offset, Title::titleStart_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Title::titleStart_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Title::changeSceneNum_ = 0;
            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Title::instance()->setState(4);
            return;
        }
    }
    else Title::titleStart_.scale = { 1,1 };

    // Tutorial
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Title::titleRule_.pos + Title::titleRule_.offset, Title::titleRule_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Title::titleRule_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Title::changeSceneNum_ = 1;
            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Title::instance()->setState(4);
            return;
        }
    }
    else Title::titleRule_.scale = { 1,1 };


    // End
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Title::titleEnd_.pos + Title::titleEnd_.offset, Title::titleEnd_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Title::titleEnd_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);
            isGameLoop = returnFalse;

            return;
        }
    }
    else Title::titleEnd_.scale = { 1,1 };


    // Omake Rabbit
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Title::titleRabbitNormal_.pos + Title::titleRabbitNormal_.offset, 
        Title::titleRabbitNormal_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Title::titleRabbitNormal_.scale = { 1.02f, 1.02f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_RABBIT_HOLD, false);
            Title::instance()->setTimer(0);
            Title::instance()->setState(5);
            return;
        }
    }
    else Title::titleRabbitNormal_.scale = { 1,1 };

}

void Judge::gameJudge()
{
    int i = 0;
    int enm = 0;

    // Enemy vs isPut
    for (i = 0; i < 4; ++i)
    {
        for (enm = 0; enm < Enemy::enemyMax; ++enm)
        {
            const VECTOR2 pos{ 190,190 };
            const VECTOR2 size{ 380, 380 };

            if (Judge::rectHitCheck(enmPos[i] - pos, size,
                Enemy::enemys_[enm]->transform()->pos() + Enemy::enemys_[enm]->transform()->offset(),
                Enemy::enemys_[enm]->collider()->hitSize()))
            {
                if (!Enemy::enemys_[enm]->exist()) continue;

                isPutEnm[i] = true;
                break;
            }

            if (enm != Enemy::enemyMax - 1) continue; // enm が enemyMax - 1 じゃなければ下の処理を飛ばす

            // 最後のenemys_がenmPos地点のrectに当たっていなければisPutEnm[i]をfalse
            if (Judge::rectHitCheck( enmPos[i] - pos , size,
                Enemy::enemys_[enm]->transform()->pos() + Enemy::enemys_[enm]->transform()->offset(),
                Enemy::enemys_[enm]->collider()->hitSize()) == false)
            {
                isPutEnm[i] = false;
            }         
        }
    }

    // Item vs Cursor
    for (auto& item : Item::items_)
    {
        if (Cursor::cursor_->isHold()) continue;
        if (!item->exist()) continue;
        if (item->type() == SHOT) continue;

        // HIT
        if (Judge::rectHitCheck(
            Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
            Cursor::cursor_->collider()->hitSize(),
            item->transform()->pos() + item->transform()->offset(),
            item->collider()->hitSize()))
        {
            // HOLD
            if (STATE(0) & PAD_LCLICK)
            {
                Cursor::cursor_->renderer()->setAnimeY(2);
                Cursor::cursor_->setIsHold(true);
                item->setIsHold(true);
                music::play(SE_RABBIT_HOLD, false);

                // TODO:滞留対策(itemをつかむごとに移動速度が増加)
                item->transform()->addVelocityX(1);
            }
        }


        for (auto& pl : Player::players_)
        {
            if (!pl->exist()) continue;

            // HIT
            if (Judge::rectHitCheck(
                pl->transform()->pos() + pl->transform()->offset(),
                pl->collider()->hitSize(),
                item->transform()->pos() + item->transform()->offset(),
                item->collider()->hitSize())
                )
            {
                // RELEASE
                if (TRG_RELEASE(0) & PAD_LCLICK)
                {
                    if (pl->isInRabbit()) continue;

                    music::play(SE_LOAD_CANNON, false);

                    item->setExist(false);
                    pl->setIsInRabbit(true);

                    pl->setElement(item->element());
                    pl->renderer()->setSprNo(shotSpr[item->element()]);
                }
            }
        }
    }

    if (STATE(0) & PAD_LCLICK)
    {
        Cursor::cursor_->setIsHold(true);
    }

    // Shot vs Enemy
    for (auto& item : Item::items_)
    {
        if (!item->exist()) continue;
        if (item->type() == ITEM) continue;
        for (auto& enemy : Enemy::enemys_)
        {
            if (!enemy->exist()) continue;
            if (enemy->transform()->pos().y < 0) continue;
            if (enemy->renderer()->sprNo() == ENEMY_M_NORMAL ||
                enemy->renderer()->sprNo() == ENEMY_M_FIRE   ||
                enemy->renderer()->sprNo() == ENEMY_M_WATER  ||
                enemy->renderer()->sprNo() == ENEMY_M_WIND)
            {
                if (enemy->actorComponent()->act() == 0 ||
                    enemy->actorComponent()->act() == 2) continue; // 移動中の場合 continue;
            }
            if (enemy->actorComponent()->act() == 0 && // act０のボスは地面に潜っているため飛ばす
                enemy->renderer()->sprNo() == BOSS_CENTER_ENTRY) continue;

            if (rectHitCheck(
                item->transform()->pos() + item->transform()->offset(), item->collider()->hitSize(),
                enemy->transform()->pos() + enemy->transform()->offset(), enemy->collider()->hitSize())
                )
            {
                item->setExist(false);

                if ((enemy->actorComponent()->act() == 0 || enemy->actorComponent()->act() == 4) 
                    && enemy->renderer()->sprNo() != BOSS_CENTER) continue; // BOSS_CENTERが出現中、または死亡中の場合はcontinue;
                if ((enemy->actorComponent()->act() == 1 || enemy->actorComponent()->act() == 6) // 動いているボスは飛ばす
                    && enemy->renderer()->sprNo() == BOSS_CENTER_ENTRY) continue;

                if ((enemy->renderer()->sprNo() == BOSS_LL_NORMAL ||
                     enemy->renderer()->sprNo() == BOSS_LL_FIRE   ||
                     enemy->renderer()->sprNo() == BOSS_LL_WATER  ||
                     enemy->renderer()->sprNo() == BOSS_LL_WIND   ||
                     enemy->renderer()->sprNo() == BOSS_RR_NORMAL ||
                     enemy->renderer()->sprNo() == BOSS_RR_FIRE   ||
                     enemy->renderer()->sprNo() == BOSS_RR_WATER  ||
                     enemy->renderer()->sprNo() == BOSS_RR_WIND)
                   && (enemy->actorComponent()->act() == 3 || enemy->actorComponent()->act() == 4)) continue; // BOSS_LLRRが死亡、蘇生中ならcontinue;

                if ((enemy->renderer()->sprNo() == BOSS_L_NORMAL ||
                     enemy->renderer()->sprNo() == BOSS_L_FIRE   ||
                     enemy->renderer()->sprNo() == BOSS_L_WATER  ||
                     enemy->renderer()->sprNo() == BOSS_L_WIND   ||
                     enemy->renderer()->sprNo() == BOSS_R_NORMAL ||
                     enemy->renderer()->sprNo() == BOSS_R_FIRE   ||
                     enemy->renderer()->sprNo() == BOSS_R_WATER  ||
                     enemy->renderer()->sprNo() == BOSS_R_WIND) 
                    && (enemy->actorComponent()->act() == 3 || enemy->actorComponent()->act() == 4)) continue; // BOSS_LRが死亡、蘇生中ならcontinue;)

                // 攻撃キャンセル
                if (enemy->renderer()->sprNo() != BOSS_CENTER   && 
                    enemy->renderer()->sprNo() != BOSS_L_NORMAL &&
                    enemy->renderer()->sprNo() != BOSS_L_FIRE   &&
                    enemy->renderer()->sprNo() != BOSS_L_WATER  &&
                    enemy->renderer()->sprNo() != BOSS_L_WIND   &&
                    enemy->renderer()->sprNo() != BOSS_R_NORMAL &&
                    enemy->renderer()->sprNo() != BOSS_R_FIRE   &&
                    enemy->renderer()->sprNo() != BOSS_R_WATER  &&
                    enemy->renderer()->sprNo() != BOSS_R_WIND   )
                {
                    enemy->renderer()->setAnimeTimer(0);
                    enemy->renderer()->setAnimeX(0);

                    enemy->renderer()->setAnimeY(1);
                    if (
                        enemy->renderer()->sprNo() == BOSS_LL_NORMAL ||
                        enemy->renderer()->sprNo() == BOSS_LL_FIRE   ||
                        enemy->renderer()->sprNo() == BOSS_LL_WATER  ||
                        enemy->renderer()->sprNo() == BOSS_LL_WIND   ||
                        enemy->renderer()->sprNo() == BOSS_RR_NORMAL ||
                        enemy->renderer()->sprNo() == BOSS_RR_FIRE   ||
                        enemy->renderer()->sprNo() == BOSS_RR_WATER  ||
                        enemy->renderer()->sprNo() == BOSS_RR_WIND   )
                    {
                        enemy->renderer()->setAnimeY(0);
                        enemy->transform()->setPos(enemy->transform()->orgPos());

                    }

                    enemy->actorComponent()->setAct(1);
                }


                const int efcSprites[] = {
                    EFFECT_NORMAL,
                    EFFECT_FIRE,
                    EFFECT_WATER,
                    EFFECT_WIND,
                    EFFECT_RARE,
                };
                int efcSprNo = efcSprites[item->element()];
                VECTOR2 efcScale = { 1,1 };

                // Rare
                if (item->element() == RARE)
                {
                    enemy->actorComponent()->subHp(20);
                    efcScale = { 3.0f, 3.0f };
                }
                // Element Advantage
                else if ((item->element() == FIRE && enemy->element() == WIND) ||
                    (item->element() == WATER && enemy->element() == FIRE) ||
                    (item->element() == WIND && enemy->element() == WATER))
                {
                    enemy->actorComponent()->subHp(4);
                    efcScale = {2.5f, 2.5f };
                }
                // Element Disadvantage
                else if ((item->element() == FIRE && enemy->element() == WATER) ||
                    (item->element() == WATER && enemy->element() == WIND) ||
                    (item->element() == WIND && enemy->element() == FIRE))
                {
                    enemy->actorComponent()->subHp(1);
                    efcScale = { 0.7f, 0.7f };
                }
                // Element Draw
                else if (item->element() == enemy->element() ||
                    item->element() == NORMAL ||
                    enemy->element() == NORMAL)
                {
                    enemy->actorComponent()->subHp(2);
                    efcScale = { 1.25f,1.25f };
                }

                // 大目玉出現時にefcScaleを大きくする
                if (enemy->renderer()->sprNo() == BOSS_CENTER) efcScale = { 2.5f,2.5f };


                if(item->element() == NORMAL ||
                   item->element() == RARE)  music::play(SE_ATK_NORMAL, false);
                if(item->element() == FIRE)  music::play(SE_ATK_FIRE,   false);
                if(item->element() == WATER) music::play(SE_ATK_WATER,  false);
                if(item->element() == WIND)  music::play(SE_ATK_WIND,   false);

                enemy->actorComponent()->setDmgTimer(10);
                if (enemy->actorComponent()->hp() > 0) enemy->actorComponent()->setAtkTimer(enemy->actorComponent()->atkTimerMax());

                OBJ2D::searchSet(Effect::effect_, Effect::efcMax, EFFECT, efcSprNo, item->transform()->pos(), efcScale, false);
                break;
            }
        }
    }
}

void Judge::overJudge()
{
    Cursor::cursor_->setIsTouchCmd(false);

    // Retry
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Over::overRetry_.pos + Over::overRetry_.offset, Over::overRetry_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Over::overRetry_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Over::isRetry_ = true;
            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Over::instance()->setState(4);
            return;
        }
    }
    else Over::overRetry_.scale = { 1,1 };


    // Title
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Over::overTitle_.pos + Over::overTitle_.offset, Over::overTitle_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Over::overTitle_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Over::isRetry_ = false;
            Enemy::enemySet_ = enemySets[0]; // waveリセット

            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Over::instance()->setState(4);
            return;
        }
    }
    else Over::overTitle_.scale = { 1,1 };
}

void Judge::clearJudge()
{
    Cursor::cursor_->setIsTouchCmd(false);

    // Retry
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Clear::clearRetry_.pos + Clear::clearRetry_.offset, Clear::clearRetry_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Clear::clearRetry_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Clear::isRetry_ = true;
            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Clear::instance()->setState(4);
            return;
        }
    }
    else Clear::clearRetry_.scale = { 1,1 };


    // Title
    if (Judge::rectHitCheck(
        Cursor::cursor_->transform()->pos() + Cursor::cursor_->transform()->offset(),
        Cursor::cursor_->collider()->hitSize(),
        Clear::clearTitle_.pos + Clear::clearTitle_.offset, Clear::clearTitle_.hitSize))
    {
        Cursor::cursor_->setIsTouchCmd(true);
        Cursor::cursor_->renderer()->setAnimeY(1);
        Clear::clearTitle_.scale = { 1.1f, 1.1f };

        if (TRG(0) & PAD_LCLICK)
        {
            music::play(SE_DECIDE, false);

            Clear::isRetry_ = false;
            Scene::isIrisEnd_ = false;
            Scene::setIrisInOut(false);
            Clear::instance()->setState(4);
            return;
        }
    }
    else Clear::clearTitle_.scale = { 1,1 };
}