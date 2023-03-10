#pragma once
#include "../GameLib/texture.h"

// ƒ‰ƒxƒ‹’è‹`
enum TEXNO
{
    CURSOR,

    TITLE_BACK,
    TITLE_CLOUD,
    TITLE_RABBIT_NORMAL,
    TITLE_REFLECTION,
    TITLE_RABBIT_FIRE,
    TITLE_RABBIT_WIND,

    TITLE_LOGO,
    TITLE_START,
    TITLE_RULE,
    TITLE_END,

    RULE_1,
    RULE_2,
    RULE_3,
    RULE_4,
    RULE_5,
    RULE_6,
    RULE_7,
    RULE_8,
    RULE_9,

    OVER_BACK,
    OVER_LOGO,
    OVER_TITLE,
    OVER_RETRY,
    
    CLEAR_BACK,
    CLEAR_LOGO,
    CLEAR_TITLE,
    CLEAR_RETRY,

    CLEAR_RABBIT,
    CLEAR_FIRE,
    CLEAR_RARE,

    GAME_BACK,
    WALL_1,
    WALL_2,
    WALL_3,
    WALL_4,

    CANNON,
    CANNON_RABBIT,
    CANNON_SHOT_NORMAL,
    CANNON_SHOT_FIRE,
    CANNON_SHOT_WATER,
    CANNON_SHOT_WIND,
    CANNON_SHOT_RARE,

    RABBIT_NORMAL,
    RABBIT_FIRE,
    RABBIT_WATER,
    RABBIT_WIND,
    RABBIT_RARE,

    RABBIT_SHOT_NORMAL,
    RABBIT_SHOT_FIRE,
    RABBIT_SHOT_WATER,
    RABBIT_SHOT_WIND,
    RABBIT_SHOT_RARE,


    ENEMY_S_NORMAL,
    ENEMY_S_FIRE,
    ENEMY_S_WATER,
    ENEMY_S_WIND,

    ENEMY_M_NORMAL,
    ENEMY_M_FIRE,
    ENEMY_M_WATER,
    ENEMY_M_WIND,

    BOSS_L_NORMAL,
    BOSS_L_FIRE,
    BOSS_L_WATER,
    BOSS_L_WIND,

    BOSS_R_NORMAL,
    BOSS_R_FIRE,
    BOSS_R_WATER,
    BOSS_R_WIND,

    BOSS_LL_NORMAL,
    BOSS_LL_FIRE,
    BOSS_LL_WATER,
    BOSS_LL_WIND,

    BOSS_RR_NORMAL,
    BOSS_RR_FIRE,
    BOSS_RR_WATER,
    BOSS_RR_WIND,

    BOSS_CENTER_ENTRY,
    BOSS_CENTER,

    CANNON_EFC,
    WALL_SMOKE_EFC,
    EFFECT_NORMAL,
    EFFECT_FIRE,
    EFFECT_WATER,
    EFFECT_WIND,
    EFFECT_RARE,

    GAUGE_NORMAL_HP,
    GAUGE_FIRE_HP,
    GAUGE_WATER_HP,
    GAUGE_WIND_HP,
    GAUGE_NORMAL,
    GAUGE_FIRE,
    GAUGE_WATER,
    GAUGE_WIND,

    GAUGE_BOSS,
    GAUGE_BOSS_DMG,

    ELEMENT,

    WAVE_1,
    WAVE_2,
    WAVE_3,
};

extern GameLib::LoadTexture loadTextureTitle[];
extern GameLib::LoadTexture loadTextureTutorial[];
extern GameLib::LoadTexture loadTextureOver[];
extern GameLib::LoadTexture loadTextureClear[];
extern GameLib::LoadTexture loadTextureGame[];