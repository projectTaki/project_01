#include "all.h"

void Audio::init()
{
 /////////////////////////BGM//////////////////////////
    music::load(BGM_TITLE,    L"./Data/Musics/Title.wav",      0.5f);
    music::load(BGM_TUTORIAL, L"./Data/Musics/Tutorial.wav",   0.5f);
    music::load(BGM_PLAY,     L"./Data/Musics/Play.wav",       0.5f);
    music::load(BGM_CLEAR,    L"./Data/Musics/Clear.wav",      4.0f);
    music::load(BGM_OVER,     L"./Data/Musics/Over.wav",       2.0f);
    music::load(BGM_BOSS,     L"./Data/Musics/Boss.wav",       0.5f);

//////////////////////////SE////////////////////////////
   music::load(SE_DECIDE,      L"./Data/Sounds/Decide.wav",         0.75f);
   music::load(SE_RABBIT_HOLD, L"./Data/Sounds/Tumamu.wav",         0.75f);

   music::load(SE_WAVE,        L"./Data/Sounds/Wave.wav",           2.0f);

   music::load(SE_LOAD_CANNON, L"./Data/Sounds/LoadCannon.wav",     0.75f);
   music::load(SE_CANNON,      L"./Data/Sounds/Cannon.wav",         3.0f);

   music::load(SE_ATK_NORMAL,  L"./Data/Sounds/Attack_Normal.wav",  4.5f);
   music::load(SE_ATK_FIRE,    L"./Data/Sounds/Attack_Fire.wav",    2.5f);
   music::load(SE_ATK_WATER,   L"./Data/Sounds/Attack_Water.wav",   2.5f);
   music::load(SE_ATK_WIND,    L"./Data/Sounds/Attack_Wind.wav",    4.0f);

   music::load(SE_RMPT_DMG,    L"./Data/Sounds/Rampart_Damage.wav", 5.0f);
   music::load(SE_HP_WARNING,  L"./Data/Sounds/Hp_Warning.wav",     6.5f);

   music::load(SE_ENM_SPAWN,   L"./Data/Sounds/Enemy_Spawn.wav",   9.0f);
   music::load(SE_ENM_DEATH,   L"./Data/Sounds/Enemy_Death.wav",   3.75f);
   music::load(SE_ENM_ATK,     L"./Data/Sounds/Enemy_Attack.wav",  5.0f);

   music::load(SE_BOSS_QUAKE,  L"./Data/Sounds/BossQuake.wav",      3.5f);
   music::load(SE_BOSS_DEATH,  L"./Data/Sounds/BossDeath.wav",      5.0f);
}

