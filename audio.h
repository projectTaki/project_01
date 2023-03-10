#pragma once
using namespace GameLib;

enum AUDIO
{
    // BGM
    BGM_TITLE,
    BGM_TUTORIAL,
    BGM_PLAY,
    BGM_OVER,
    BGM_CLEAR,
    BGM_BOSS,


    // SE
    SE_DECIDE,
    SE_RABBIT_HOLD,

    SE_WAVE,

    SE_LOAD_CANNON,
    SE_CANNON,

    SE_ATK_NORMAL,
    SE_ATK_FIRE,
    SE_ATK_WATER,
    SE_ATK_WIND,

    SE_HP_WARNING,
    SE_RMPT_DMG, // èÈï«É_ÉÅÅ[ÉW

    SE_ENM_SPAWN,
    SE_ENM_DEATH,
    SE_ENM_ATK,

    SE_BOSS_QUAKE,
    SE_BOSS_DEATH,
};

class Audio
{
public:
 static void init();

};