#include "all.h"

GameLib::LoadTexture loadTextureTitle[] = {
    { TEXNO::CURSOR,  L"./Data/Images/Cursor.png",  1U },

    { TEXNO::TITLE_BACK,          L"./Data/Images/Title/Title_Back.png",          1U },
    { TEXNO::TITLE_CLOUD,         L"./Data/Images/Title/Title_Cloud.png",         1U },
    { TEXNO::TITLE_RABBIT_NORMAL, L"./Data/Images/Title/Title_Rabbit_Normal.png", 1U },
    { TEXNO::TITLE_REFLECTION,    L"./Data/Images/Title/Title_Reflection.png",    1U },
    { TEXNO::TITLE_RABBIT_FIRE,   L"./Data/Images/Title/Title_Rabbit_Fire.png",   1U },
    { TEXNO::TITLE_RABBIT_WIND,   L"./Data/Images/Title/Title_Rabbit_Wind.png",   1U },

    { TEXNO::TITLE_LOGO,  L"./Data/Images/Title/Title_Logo.png",  1U },
    { TEXNO::TITLE_START, L"./Data/Images/Title/Title_Start.png", 1U },
    { TEXNO::TITLE_RULE,  L"./Data/Images/Title/Title_Rule.png",  1U },
    { TEXNO::TITLE_END,   L"./Data/Images/Title/Title_End.png",   1U },

    // irisShot用
    { TEXNO::RABBIT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Normal.png", 1U },
    { TEXNO::RABBIT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Fire.png",   1U },
    { TEXNO::RABBIT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Water.png",  1U },
    { TEXNO::RABBIT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Wind.png",   1U },
    { TEXNO::RABBIT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Rare.png",   1U },

    { -1, nullptr }	// 終了フラグ
};

GameLib::LoadTexture loadTextureTutorial[] = {
    { TEXNO::CURSOR,  L"./Data/Images/Cursor.png",  1U },

    { TEXNO::ELEMENT, L"./Data/Images/Game/UI/Element.png", 1U },

    { TEXNO::RULE_1,  L"./Data/Images/Rule/Rule_1.png",  1U },
    { TEXNO::RULE_2,  L"./Data/Images/Rule/Rule_2.png",  1U },
    { TEXNO::RULE_3,  L"./Data/Images/Rule/Rule_3.png",  1U },
    { TEXNO::RULE_4,  L"./Data/Images/Rule/Rule_4.png",  1U },
    { TEXNO::RULE_5,  L"./Data/Images/Rule/Rule_5.png",  1U },
    { TEXNO::RULE_6,  L"./Data/Images/Rule/Rule_6.png",  1U },
    { TEXNO::RULE_7,  L"./Data/Images/Rule/Rule_7.png",  1U },
    { TEXNO::RULE_8,  L"./Data/Images/Rule/Rule_8.png",  1U },
    { TEXNO::RULE_9,  L"./Data/Images/Rule/Rule_9.png",  1U },

    // irisShot用
    { TEXNO::RABBIT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Normal.png", 1U },
    { TEXNO::RABBIT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Fire.png",   1U },
    { TEXNO::RABBIT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Water.png",  1U },
    { TEXNO::RABBIT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Wind.png",   1U },
    { TEXNO::RABBIT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Rare.png",   1U },

    { -1, nullptr }	// 終了フラグ
};

GameLib::LoadTexture loadTextureGame[] = {
    { TEXNO::CURSOR,  L"./Data/Images/Cursor.png",  1U },   

    { TEXNO::GAME_BACK, L"./Data/Images/Game/BackGrounds/Game_Back.png", 2U },
    { TEXNO::WALL_1,    L"./Data/Images/Game/BackGrounds/Wall_100.png", 2U },
    { TEXNO::WALL_2,    L"./Data/Images/Game/BackGrounds/Wall_75.png",  2U },
    { TEXNO::WALL_3,    L"./Data/Images/Game/BackGrounds/Wall_50.png",  2U },
    { TEXNO::WALL_4,    L"./Data/Images/Game/BackGrounds/Wall_25.png",  2U },
    
    { TEXNO::CANNON,             L"./Data/Images/Game/Cannons/Cannon.png",             1U },
    { TEXNO::CANNON_RABBIT,      L"./Data/Images/Game/Cannons/Cannon_Rabbit.png",      1U },
    { TEXNO::CANNON_SHOT_NORMAL, L"./Data/Images/Game/Cannons/Cannon_Shot_Normal.png", 1U },
    { TEXNO::CANNON_SHOT_FIRE,   L"./Data/Images/Game/Cannons/Cannon_Shot_Fire.png",   1U },
    { TEXNO::CANNON_SHOT_WATER,  L"./Data/Images/Game/Cannons/Cannon_Shot_Water.png",  1U },
    { TEXNO::CANNON_SHOT_WIND,   L"./Data/Images/Game/Cannons/Cannon_Shot_Wind.png",   1U },
    { TEXNO::CANNON_SHOT_RARE,   L"./Data/Images/Game/Cannons/Cannon_Shot_Rare.png",   1U },
    
    { TEXNO::RABBIT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Normal.png", 1U },
    { TEXNO::RABBIT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Fire.png",   1U },
    { TEXNO::RABBIT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Water.png",  1U },
    { TEXNO::RABBIT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Wind.png",   1U },
    { TEXNO::RABBIT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Rare.png",   1U },
    
    { TEXNO::RABBIT_SHOT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Shot_Normal.png", 1U },
    { TEXNO::RABBIT_SHOT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Shot_Fire.png",   1U },
    { TEXNO::RABBIT_SHOT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Shot_Water.png",  1U },
    { TEXNO::RABBIT_SHOT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Shot_Wind.png",   1U },
    { TEXNO::RABBIT_SHOT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Shot_Rare.png",   1U }, 

    { TEXNO::ENEMY_S_NORMAL, L"./Data/Images/Game/Enemys/Enemy_S_Normal.png", 1U },
    { TEXNO::ENEMY_S_FIRE,   L"./Data/Images/Game/Enemys/Enemy_S_Fire.png",   1U },
    { TEXNO::ENEMY_S_WATER,  L"./Data/Images/Game/Enemys/Enemy_S_Water.png",  1U },
    { TEXNO::ENEMY_S_WIND,   L"./Data/Images/Game/Enemys/Enemy_S_Wind.png",   1U },
    
    { TEXNO::ENEMY_M_NORMAL, L"./Data/Images/Game/Enemys/Enemy_M_Normal.png", 1U },
    { TEXNO::ENEMY_M_FIRE,   L"./Data/Images/Game/Enemys/Enemy_M_Fire.png",   1U },
    { TEXNO::ENEMY_M_WATER,  L"./Data/Images/Game/Enemys/Enemy_M_Water.png",  1U },
    { TEXNO::ENEMY_M_WIND,   L"./Data/Images/Game/Enemys/Enemy_M_Wind.png",   1U },
    
    { TEXNO::BOSS_L_NORMAL,  L"./Data/Images/Game/Bosses/Boss_L_Normal.png",  1U },
    { TEXNO::BOSS_L_FIRE,    L"./Data/Images/Game/Bosses/Boss_L_Fire.png",    1U },
    { TEXNO::BOSS_L_WATER,   L"./Data/Images/Game/Bosses/Boss_L_Water.png",   1U },
    { TEXNO::BOSS_L_WIND,    L"./Data/Images/Game/Bosses/Boss_L_Wind.png",    1U },

    { TEXNO::BOSS_R_NORMAL,  L"./Data/Images/Game/Bosses/Boss_R_Normal.png",  1U },
    { TEXNO::BOSS_R_FIRE,    L"./Data/Images/Game/Bosses/Boss_R_Fire.png",    1U },
    { TEXNO::BOSS_R_WATER,   L"./Data/Images/Game/Bosses/Boss_R_Water.png",   1U },
    { TEXNO::BOSS_R_WIND,    L"./Data/Images/Game/Bosses/Boss_R_Wind.png",    1U },

    { TEXNO::BOSS_LL_NORMAL, L"./Data/Images/Game/Bosses/Boss_LL_Normal.png", 1U },
    { TEXNO::BOSS_LL_FIRE,   L"./Data/Images/Game/Bosses/Boss_LL_Fire.png",   1U },
    { TEXNO::BOSS_LL_WATER,  L"./Data/Images/Game/Bosses/Boss_LL_Water.png",  1U },
    { TEXNO::BOSS_LL_WIND,   L"./Data/Images/Game/Bosses/Boss_LL_Wind.png",   1U },

    { TEXNO::BOSS_RR_NORMAL, L"./Data/Images/Game/Bosses/Boss_RR_Normal.png", 1U },
    { TEXNO::BOSS_RR_FIRE,   L"./Data/Images/Game/Bosses/Boss_RR_Fire.png",   1U },
    { TEXNO::BOSS_RR_WATER,  L"./Data/Images/Game/Bosses/Boss_RR_Water.png",  1U },
    { TEXNO::BOSS_RR_WIND,   L"./Data/Images/Game/Bosses/Boss_RR_Wind.png",   1U },

    { TEXNO::BOSS_CENTER_ENTRY, L"./Data/Images/Game/Bosses/Boss_Center_Entry.png", 1U },
    { TEXNO::BOSS_CENTER,       L"./Data/Images/Game/Bosses/Boss_Center.png",       1U },   

    { TEXNO::CANNON_EFC,     L"./Data/Images/Game/Effects/Cannon_Shot_Effect.png", 1U },
    { TEXNO::WALL_SMOKE_EFC, L"./Data/Images/Game/Effects/Wall_Smoke_Effect.png",  1U },
    { TEXNO::EFFECT_NORMAL,  L"./Data/Images/Game/Effects/Effect_Normal.png", 1U },
    { TEXNO::EFFECT_FIRE,    L"./Data/Images/Game/Effects/Effect_Fire.png",   1U },
    { TEXNO::EFFECT_WATER,   L"./Data/Images/Game/Effects/Effect_Water.png",  1U },
    { TEXNO::EFFECT_WIND,    L"./Data/Images/Game/Effects/Effect_Wind.png",   1U },
    { TEXNO::EFFECT_RARE,    L"./Data/Images/Game/Effects/Effect_Rare.png",   1U },
    
    { TEXNO::GAUGE_NORMAL_HP, L"./Data/Images/Game/UI/Gauge_Normal_Hp.png", 1U },
    { TEXNO::GAUGE_FIRE_HP,   L"./Data/Images/Game/UI/Gauge_Fire_Hp.png",   1U },
    { TEXNO::GAUGE_WATER_HP,  L"./Data/Images/Game/UI/Gauge_Water_Hp.png",  1U },
    { TEXNO::GAUGE_WIND_HP,   L"./Data/Images/Game/UI/Gauge_Wind_Hp.png",   1U },

    { TEXNO::GAUGE_NORMAL,    L"./Data/Images/Game/UI/Gauge_Normal.png",    1U },
    { TEXNO::GAUGE_FIRE,      L"./Data/Images/Game/UI/Gauge_Fire.png",      1U },
    { TEXNO::GAUGE_WATER,     L"./Data/Images/Game/UI/Gauge_Water.png",     1U },
    { TEXNO::GAUGE_WIND,      L"./Data/Images/Game/UI/Gauge_Wind.png",      1U },

    { TEXNO::GAUGE_BOSS,     L"./Data/Images/Game/UI/Gauge_Boss_Center.png",        1U },
    { TEXNO::GAUGE_BOSS_DMG, L"./Data/Images/Game/UI/Gauge_Boss_Center_Damage.png", 1U },
    
    { TEXNO::ELEMENT, L"./Data/Images/Game/UI/Element.png", 1U },

    { TEXNO::WAVE_1, L"./Data/Images/Game/UI/Wave1.png", 1U },
    { TEXNO::WAVE_2, L"./Data/Images/Game/UI/Wave2.png", 1U },
    { TEXNO::WAVE_3, L"./Data/Images/Game/UI/Wave3.png", 1U },
    
    { -1, nullptr }	// 終了フラグ
};

GameLib::LoadTexture loadTextureOver[] = {
    { TEXNO::CURSOR,  L"./Data/Images/Cursor.png",  1U },

    { TEXNO::OVER_BACK,     L"./Data/Images/Over/Over_Back.png",     1U },
    { TEXNO::OVER_LOGO,     L"./Data/Images/Over/Over_Logo.png",     1U },
    { TEXNO::OVER_TITLE,    L"./Data/Images/Over/Over_Title.png",    1U },
    { TEXNO::OVER_RETRY,    L"./Data/Images/Over/Over_Retry.png",    1U },

    // irisShot用
    { TEXNO::RABBIT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Normal.png", 1U },
    { TEXNO::RABBIT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Fire.png",   1U },
    { TEXNO::RABBIT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Water.png",  1U },
    { TEXNO::RABBIT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Wind.png",   1U },
    { TEXNO::RABBIT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Rare.png",   1U },

    { -1, nullptr }	// 終了フラグ
};

GameLib::LoadTexture loadTextureClear[] = {
    { TEXNO::CURSOR,  L"./Data/Images/Cursor.png",  1U },

    { TEXNO::CLEAR_BACK,     L"./Data/Images/Clear/Clear_Back.png",     1U },
    { TEXNO::CLEAR_LOGO,     L"./Data/Images/Clear/Clear_Logo.png",     1U },
    { TEXNO::CLEAR_TITLE,    L"./Data/Images/Clear/Clear_Title.png",    1U },
    { TEXNO::CLEAR_RETRY,    L"./Data/Images/Clear/Clear_Retry.png",    1U },

    { TEXNO::CLEAR_RABBIT,   L"./Data/Images/Clear/Clear_Rabbit.png",   1U },
    { TEXNO::CLEAR_FIRE,     L"./Data/Images/Clear/Clear_Fire.png",     1U },
    { TEXNO::CLEAR_RARE,     L"./Data/Images/Clear/Clear_Rare.png",     1U },

    // irisShot用
    { TEXNO::RABBIT_NORMAL, L"./Data/Images/Game/Rabbits/Rabbit_Normal.png", 1U },
    { TEXNO::RABBIT_FIRE,   L"./Data/Images/Game/Rabbits/Rabbit_Fire.png",   1U },
    { TEXNO::RABBIT_WATER,  L"./Data/Images/Game/Rabbits/Rabbit_Water.png",  1U },
    { TEXNO::RABBIT_WIND,   L"./Data/Images/Game/Rabbits/Rabbit_Wind.png",   1U },
    { TEXNO::RABBIT_RARE,   L"./Data/Images/Game/Rabbits/Rabbit_Rare.png",   1U },

    { -1, nullptr }	// 終了フラグ
};
