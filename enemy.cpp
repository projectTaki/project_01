#include "all.h"
#include <sstream> // ostringstream
#include <iomanip> // setw(), setfill()

bool isPutEnm[4] = {
   false,
   false,
   false,
   false,
};

EnemySet enemySet_1[] = {
	{1, 0, ENEMY, ENEMY_S_FIRE, enmPos[1]},
	{1, 0, ENEMY, ENEMY_S_WIND, enmPos[2]},

	{2, 0, ENEMY, ENEMY_S_WIND,  enmPos[0]},
	{2, 0, ENEMY, ENEMY_S_WATER, enmPos[3]},

	{3, 0,		ENEMY, ENEMY_S_WIND,   enmPos[0]},
	{3, 2 * 60, ENEMY, ENEMY_S_FIRE,   enmPos[1]},
	{3, 4 * 60, ENEMY, ENEMY_S_WATER,  enmPos[2]},
	{3, 6 * 60, ENEMY, ENEMY_S_NORMAL, enmPos[3]},

	{4, 0, ENEMY, ENEMY_M_NORMAL, enmPos[2]},

	{-1, -1, -1, -1, {-1, -1} },
};
EnemySet enemySet_2[] = {
	{1, 0, ENEMY, ENEMY_M_WIND, enmPos[1] },
	{1, 0, ENEMY, ENEMY_S_FIRE, enmPos[3] },	
	
	{2, 0, ENEMY, ENEMY_M_WATER, enmPos[0] },
	{2, 0, ENEMY, ENEMY_M_FIRE,  enmPos[3] },	
	
	{3, 0, ENEMY, ENEMY_M_WIND,   enmPos[0] },
	{3, 0, ENEMY, ENEMY_M_WATER,  enmPos[1] },	
	{3, 0, ENEMY, ENEMY_M_FIRE,   enmPos[2] },
	{3, 0, ENEMY, ENEMY_M_NORMAL, enmPos[3] },

	{-1, -1, -1, -1, {-1, -1} },
};
EnemySet enemySet_boss[] = {
	{1, 0, ENEMY, BOSS_LL_NORMAL,     {enmPos[0].x, enmPos[0].y + 1} },
	{1, 0, ENEMY, BOSS_L_NORMAL,      enmPos[1]},
	{1, 0, ENEMY, BOSS_R_NORMAL,      enmPos[2]},
	{1, 0, ENEMY, BOSS_RR_NORMAL,     {enmPos[3].x, enmPos[3].y}     },
	{1, 0, ENEMY, BOSS_CENTER_ENTRY,  {960, 210} }, // center

	{-1, -1, -1, -1, {-1, -1} },
};
EnemySet* enemySets[] = {
	enemySet_1,
	enemySet_2,
	enemySet_boss,
	nullptr,
};

EnemySet* Enemy::enemySet_ = {};

int		Enemy::enmRound_			= 0;
int		Enemy::existEnmNum_			= 0;
bool	Enemy::endEnm_[enemyMax]	= {};
		  
bool    Enemy::killAllEnm_	= false;
int     Enemy::bossLRNum_	= 0;
VECTOR2 Enemy::moveBoss_	= {};

OBJ2D* Enemy::enemys_[enemyMax] = {};

VECTOR2 gaugeBossAnime = {};
int     gaugeDmgAnimeTimer = 0;
int     gaugeDmgAnimeY = 0;

void Enemy::init()
{
	state_ = 0;
	timer_ = 0;

	for (auto& enemy : enemys_)
	{
		enemy = new OBJ2D(
			new Transform,
			new Renderer,
			new Collider,
			new ActorComponent
		);
	}
}

void Enemy::deinit()
{
	for (auto& enemy : enemys_) safe_delete(enemy);
}

void Enemy::update() 
{
	static int stageNum = -1;

	switch (state_)
	{
	case 0:
		if (!Over::isRetry_) enemySet_ = enemySets[2];
		Over::isRetry_ = false; // リトライ判定をリセット

		++state_;
		/*fallthrough*/
		[[fallthrough]];
	case 1:
		timer_ = 0;

		enmRound_ = 1;
		existEnmNum_ = 0;

		killAllEnm_ = false;
		bossLRNum_ = 0;
		moveBoss_ = {};

		gaugeBossAnime = {};
		gaugeDmgAnimeTimer = 0;
		gaugeDmgAnimeY = 0;

		for (auto& enm : enemys_) resetEnemy(enm);
		for (auto& end : endEnm_) end = false;

		++state_;
		/*fallthrough*/
		[[fallthrough]];
	case 2:

		//if (GetAsyncKeyState('1') < 0) { stageNum = 0; state_ = 0; break; }
		//if (GetAsyncKeyState('2') < 0) { stageNum = 1; state_ = 0; break; }
		//if (GetAsyncKeyState('3') < 0) { stageNum = 2; state_ = 0; break; }
		//if (stageNum != -1) enemySet_ = enemySets[stageNum];
		//stageNum = -1;

		if (enmRound_ >= 74)
		{
				 if (enemySet_ == enemySets[0]) enemySet_ = enemySets[1];
			else if (enemySet_ == enemySets[1]) enemySet_ = enemySets[2];
			else if (enemySet_ == enemySets[2]) enemySet_ = enemySets[3];

			if (enemySet_) // next wave
			{
				music::play(SE_WAVE, false);
				Game::instance()->setState(5); // wave moveIn
				state_ = 1;
			}
			else
			{
				timer_ = 0;
				state_ = 3; // gameClear
			}

			break;
		}

		// 生成
		for (int i = 0; ; ++i)
		{
			if (enemySet_[i].round == -1) break;
			if (enemySet_[i].round != enmRound_) continue;
			if (timer_ < enemySet_[i].timer) continue;
			if (endEnm_[i]) continue;

			resetEnemy(enemys_[i]);
			enemys_[i]->renderer()->setSprNo(enemySet_[i].sprNo);
			OBJ2D::searchSet(enemys_[i], ENEMY, enemySet_[i].pos, true);

			++existEnmNum_;
			endEnm_[i] = true; // 再出現防止
			break;
		}

		if (killAllEnm_) killAllEnm();

		// 更新
		for (auto& enemy : enemys_)
		{
			if (!enemy->exist() && enemy->state() > 0)
			{
				resetEnemy(enemy);

				--existEnmNum_;
				continue;
			}
			if (enemy->actorComponent()->hp() <= 0) enemy->actorComponent()->setHp(0);
			if (!enemy->exist()) continue;


			switch (enemy->renderer()->sprNo())
			{
			case ENEMY_S_NORMAL:
			case ENEMY_S_FIRE:
			case ENEMY_S_WATER:
			case ENEMY_S_WIND:	 enmS(enemy); break;

			case ENEMY_M_NORMAL:
			case ENEMY_M_FIRE:
			case ENEMY_M_WATER:
			case ENEMY_M_WIND:	 enmM(enemy); break;

			case BOSS_L_NORMAL:
			case BOSS_L_FIRE:
			case BOSS_L_WATER:
			case BOSS_L_WIND:
			case BOSS_R_NORMAL:
			case BOSS_R_FIRE:
			case BOSS_R_WATER:
			case BOSS_R_WIND:		 bossLR(enemy); break;

			case BOSS_LL_NORMAL:
			case BOSS_LL_FIRE:
			case BOSS_LL_WATER:
			case BOSS_LL_WIND:
			case BOSS_RR_NORMAL:
			case BOSS_RR_FIRE:	
			case BOSS_RR_WATER:	
			case BOSS_RR_WIND:		 bossLLRR(enemy); break;

			case BOSS_CENTER_ENTRY:	 
			case BOSS_CENTER:	 bossCenter(enemy); break;
			}

			if (enemy->actorComponent()->dmgTimer() > 0) enemy->actorComponent()->subDmgTimer(1);

			if (enemy->actorComponent()->atkTimer() > 0 &&
				enemy->actorComponent()->hp()       > 0 &&
				enemy->actorComponent()->act()      > 0)
			{
				if (enemy->renderer()->sprNo() == ENEMY_M_NORMAL  || // Mサイズの敵かつ移動中でない場合
					enemy->renderer()->sprNo() == ENEMY_M_FIRE    ||
					enemy->renderer()->sprNo() == ENEMY_M_WATER   ||
					enemy->renderer()->sprNo() == ENEMY_M_WIND)
				{
					if (enemy->actorComponent()->act() != 2) enemy->actorComponent()->subAtkTimer(1);
				}
				else if (bossLRNum_ != 2) enemy->actorComponent()->subAtkTimer(1);
				
				if (bossLRNum_ == 2) enemy->actorComponent()->setAtkTimer(enemy->actorComponent()->atkTimerMax());
			}

		}


		++timer_;

		// add Round
		if (existEnmNum_ <= 0)
		{
			if (existEnmNum_ < 0) existEnmNum_ = 0;

			timer_ = 0; // タイマーリセット
			++enmRound_;
		}

		break;

	case 3: // gameClear
		++timer_;

		if (timer_ == 2 * 60)
		{
			Scene::isIrisEnd_ = false;
			Scene::setIrisInOut(false);
			Game::instance()->setState(6);
			break;
		}

		break;
	}
}


void Enemy::render()
{
	VECTOR2 copyPos[enemyMax]{};
	float copyHp[enemyMax]{};
	float copyAtkTimer[enemyMax]{};
	int i = 0;

	for (i = 0; i < enemyMax; ++i)
	{
		if (!enemys_[i]->exist()) continue;

		VECTOR4 enmRedColor[enemyMax] = {};
		if (enemys_[i]->actorComponent()->dmgTimer() > 0)
		{
			enmRedColor[i] = { 0, -0.7f, -0.7f,0 };
		}

		texture::begin(enemys_[i]->renderer()->sprNo());
		texture::draw(
			enemys_[i]->renderer()->sprNo(),
			enemys_[i]->transform()->pos() + BG::bgQuakePos(),
			enemys_[i]->transform()->scale(),
			enemys_[i]->transform()->texPos(),
			enemys_[i]->collider()->size(),
			enemys_[i]->transform()->center(),
			enemys_[i]->transform()->angle(),
			enemys_[i]->renderer()->color() + enmRedColor[i]
		);
		texture::end(enemys_[i]->renderer()->sprNo());


		copyPos[i] = enemys_[i]->transform()->pos();
		//if(enemys_[i]->renderer()->sprNo() == ENEMY_S_NORMAL ||
		//	enemys_[i]->renderer()->sprNo() == ENEMY_S_FIRE  || 
		//	enemys_[i]->renderer()->sprNo() == ENEMY_S_WATER || 
		//	enemys_[i]->renderer()->sprNo() == ENEMY_S_WIND) copyPos[i] = enemys_[i]->transform()->orgPos();


		// ※static_cast<float>の()の中に割る値（/ hpMax）は入れないこと
		// (ゲージ表示がバグる)
		copyHp[i] = static_cast<float>(enemys_[i]->actorComponent()->hp()) 
				  / static_cast<float>(enemys_[i]->actorComponent()->hpMax());
		if (enemys_[i]->actorComponent()->hp() <= 0) copyHp[i] = 0;

		 copyAtkTimer[i] = static_cast<float>(enemys_[i]->actorComponent()->atkTimer()) 
						 / static_cast<float>(enemys_[i]->actorComponent()->atkTimerMax());

		if (Scene::isDispHitRect_)
		{
			primitive::rect(
				enemys_[i]->transform()->pos() + enemys_[i]->transform()->offset(),
				enemys_[i]->collider()->hitSize(),
				{ 0,0 }, 0, { 1, 0, 0, 0.2f }
			);			
		}
	}

	// hp, attack, stick UI
	for (i = 0; i < enemyMax; ++i)
	{
		static float gaugeAlpha[enemyMax] = {};
		// Controll gaugeAlpha
		if ((enemys_[i]->renderer()->sprNo() == ENEMY_M_NORMAL ||
			enemys_[i]->renderer()->sprNo()  == ENEMY_M_FIRE   ||
			enemys_[i]->renderer()->sprNo()  == ENEMY_M_WATER  ||
			enemys_[i]->renderer()->sprNo()  == ENEMY_M_WIND)  &&
			enemys_[i]->actorComponent()->act() == 2)
		{
			if (gaugeAlpha[i] > 0)
			{
				gaugeAlpha[i] += -0.075f;
				if (gaugeAlpha[i] < 0) gaugeAlpha[i] = 0;
			}
		}
		else if (enemys_[i]->renderer()->sprNo() == BOSS_CENTER_ENTRY &&
				(enemys_[i]->actorComponent()->act() == 0 ||
				 enemys_[i]->actorComponent()->act() == 1 ||
				 enemys_[i]->actorComponent()->act() == 6))
		{
			if (gaugeAlpha[i] > 0)
			{
				gaugeAlpha[i] += -1.0f;
				if (gaugeAlpha[i] < 0) gaugeAlpha[i] = 0;
			}
		}
		else if (enemys_[i]->actorComponent()->hp() <= 0)
		{
			if (enemys_[i]->renderer()->sprNo() != BOSS_CENTER ||
				enemys_[i]->actorComponent()->act() != 7)
			{
				if (gaugeAlpha[i] > 0)
				{
					gaugeAlpha[i] += -0.075f;
					if (gaugeAlpha[i] < 0) gaugeAlpha[i] = 0;
				}
			}
		}
		else
		{
			if (gaugeAlpha[i] < 1)
			{
				gaugeAlpha[i] += 0.075f;
				if (gaugeAlpha[i] > 1) gaugeAlpha[i] = 1;
			}
		}



		if (!enemys_[i]->exist()) continue;


		// hp
		if (enemys_[i]->renderer()->sprNo() == BOSS_CENTER ||
			enemys_[i]->renderer()->sprNo() == BOSS_CENTER_ENTRY)

		{
			const VECTOR2 gaugeBossPos = {-370, 150};

			// ゲージ増減
			static float length[enemyMax] = {};
			static float currentHpGauge[enemyMax] = {};
			static float redHpGauge[enemyMax] = {};

			length[i] = copyHp[i] * 600;

			// HPゲージ増減
			if (currentHpGauge[i] < length[i])
			{
				currentHpGauge[i] += 5.0f;
				if (currentHpGauge[i] > length[i]) currentHpGauge[i] = length[i];
			}
			if (currentHpGauge[i] > length[i])
			{
				currentHpGauge[i] -= 5.0f;
				if (currentHpGauge[i] < length[i]) currentHpGauge[i] = length[i];
			}
			// スリップゲージ増減
			if (redHpGauge[i] < currentHpGauge[i])
			{
				redHpGauge[i] = currentHpGauge[i];
			}
			if (redHpGauge[i] > currentHpGauge[i])
			{
				if (copyHp[i] <= 0) redHpGauge[i] -= 10.0f;
				else redHpGauge[i] -= 0.15f;

				if (redHpGauge[i] < currentHpGauge[i]) redHpGauge[i] = currentHpGauge[i];
			}


			if (enemys_[i]->actorComponent()->act() != 7) gaugeBossAnime = {};

				 if (enemys_[i]->actorComponent()->act() == 3) gaugeDmgAnimeY = 0;
			else if (enemys_[i]->actorComponent()->act() == 4) gaugeDmgAnimeY = 1;
			else if (enemys_[i]->actorComponent()->act() == 5) gaugeDmgAnimeY = 2;
			else gaugeDmgAnimeY = -1;
				 
			if (enemys_[i]->actorComponent()->hp() <= 0)
			{
				gaugeBossAnime.y = 1;
				if (gaugeBossAnime.x < 4)
				{
					gaugeBossAnime.x = static_cast<float>(gaugeDmgAnimeTimer / 12);
					++gaugeDmgAnimeTimer;
				}
			}

			texture::begin(GAUGE_BOSS);
			texture::draw(
				GAUGE_BOSS,
				VECTOR2(copyPos[i].x + gaugeBossPos.x, copyPos[i].y + gaugeBossPos.y) + (BG::bgQuakePos() * 0.2f),
				{ 1,1 }, { 700* gaugeBossAnime.x, 150* gaugeBossAnime.y }, { 700,150 },
				{ 0,0 }, ToRadian(0), { 1,1,1,gaugeAlpha[i] }
			);
			texture::end(GAUGE_BOSS);

			// スリップゲージ
			primitive::rect(
				VECTOR2(copyPos[i].x + (gaugeBossPos.x + 80), copyPos[i].y + (gaugeBossPos.y + 25)) + (BG::bgQuakePos() * 0.2f),
				{ redHpGauge[i], 25 },
				{ 0,0 },
				ToRadian(0),
				{ 0.9f, 0.3f, 0.0f, gaugeAlpha[i] }
			);	
			// HPゲージ
			primitive::rect(
				VECTOR2(copyPos[i].x + gaugeBossPos.x + 80, copyPos[i].y + gaugeBossPos.y + 25) + (BG::bgQuakePos() * 0.2f),
				{ currentHpGauge[i], 25 },
				{ 0,0 },
				ToRadian(0),
				{ 0.0f, 0.9f, 0.5f, gaugeAlpha[i] }
			);				
			// HPゲージ影
			primitive::rect(
				VECTOR2(copyPos[i].x + gaugeBossPos.x + 80, copyPos[i].y + gaugeBossPos.y + 30) + (BG::bgQuakePos() * 0.2f),
				{ currentHpGauge[i], 20 },
				{ 0,0 },
				ToRadian(0),
				{ 0.0f, 0.7f, 0.3f, gaugeAlpha[i] }
			);			

			// HP, スリップゲージの先端
			if (currentHpGauge[i] > 0)
			{
				primitive::rect(
					VECTOR2(copyPos[i].x + gaugeBossPos.x + 80, copyPos[i].y + gaugeBossPos.y + 30) + (BG::bgQuakePos() * 0.2f),
					{ redHpGauge[i] + 5, 15 },
					{ 0,0 },
					ToRadian(0),
					{ 0.9f, 0.3f, 0.0f, gaugeAlpha[i] }
				);				
				primitive::rect(
					VECTOR2(copyPos[i].x + gaugeBossPos.x + 80, copyPos[i].y + gaugeBossPos.y + 30) + (BG::bgQuakePos() * 0.2f),
					{ currentHpGauge[i] + 5, 15 },
					{ 0,0 },
					ToRadian(0),
					{ 0.0f, 0.9f, 0.5f, gaugeAlpha[i] }
				);				
				primitive::rect(
					VECTOR2(copyPos[i].x + gaugeBossPos.x + 80, copyPos[i].y + gaugeBossPos.y + 40) + (BG::bgQuakePos() * 0.2f),
					{ currentHpGauge[i] + 5, 5 },
					{ 0,0 },
					ToRadian(0),
					{ 0.0f, 0.7f, 0.3f, gaugeAlpha[i] }
				);
			}

			texture::begin(GAUGE_BOSS_DMG);
			texture::draw(
				GAUGE_BOSS_DMG,
				VECTOR2(copyPos[i].x + gaugeBossPos.x, copyPos[i].y + gaugeBossPos.y) + (BG::bgQuakePos() * 0.2f),
				{ 1,1 }, {0 ,150 * static_cast<float>(gaugeDmgAnimeY) }, { 700,150 },
				{ 0,0 }, ToRadian(0), { 1,1,1,gaugeAlpha[i] }
			);
			texture::end(GAUGE_BOSS_DMG);
		}
		else
		{
			const VECTOR2 pos = { 10, 200 };

			const int gaugeSprites[] = {
				GAUGE_NORMAL,
				GAUGE_FIRE,
				GAUGE_WATER,
				GAUGE_WIND,
			};
			int gaugeSprNo[enemyMax] = {};
				 if (enemys_[i]->renderer()->sprNo() == BOSS_L_NORMAL ||
					 enemys_[i]->renderer()->sprNo() == BOSS_R_NORMAL ) gaugeSprNo[i] = GAUGE_NORMAL_HP;
			else if (enemys_[i]->renderer()->sprNo() == BOSS_L_FIRE   ||
					 enemys_[i]->renderer()->sprNo() == BOSS_R_FIRE   ) gaugeSprNo[i] = GAUGE_FIRE_HP;
			else if (enemys_[i]->renderer()->sprNo() == BOSS_L_WATER  ||
					 enemys_[i]->renderer()->sprNo() == BOSS_R_WATER  ) gaugeSprNo[i] = GAUGE_WATER_HP;
			else if (enemys_[i]->renderer()->sprNo() == BOSS_L_WIND   ||
					 enemys_[i]->renderer()->sprNo() == BOSS_R_WIND   ) gaugeSprNo[i] = GAUGE_WIND_HP;
			else gaugeSprNo[i] = gaugeSprites[enemys_[i]->element()];

			// ゲージ増減
			static float length[enemyMax] = {};
			static float currentHpGauge[enemyMax] = {};
			static float redHpGauge[enemyMax] = {};

			length[i] = copyHp[i] * 165;

			// HPゲージ増減
			if (currentHpGauge[i] < length[i]) 
			{
				currentHpGauge[i] += 2.0;
				if (currentHpGauge[i] > length[i]) currentHpGauge[i] = length[i];
			}
			if (currentHpGauge[i] > length[i])
			{
				if (copyHp[i] <= 0) currentHpGauge[i] -= 20.0f;
				else currentHpGauge[i] -= 15.0f;
				if (currentHpGauge[i] < length[i]) currentHpGauge[i] = length[i];
			}

			// スリップ？ゲージ増減
			if (redHpGauge[i] < currentHpGauge[i])
			{
				redHpGauge[i] = currentHpGauge[i];
			}
			if (redHpGauge[i] > currentHpGauge[i])
			{
				if(copyHp[i] <= 0) redHpGauge[i] -= 10.0f;
				else redHpGauge[i] -= 0.9f;

				if (redHpGauge[i] < currentHpGauge[i]) redHpGauge[i] = currentHpGauge[i];
			}


			texture::begin(gaugeSprNo[i]);
			texture::draw(
				gaugeSprNo[i],
				VECTOR2(copyPos[i].x + pos.x, copyPos[i].y - pos.y) + (BG::bgQuakePos() * 0.2f),
				{ 1,1 }, { 0,0 }, { 255,140 },
				{ 0,0 }, ToRadian(0), { 1,1,1,gaugeAlpha[i] }
			);
			texture::end(gaugeSprNo[i]);

			// スリップゲージ
			primitive::rect(
				VECTOR2(copyPos[i].x + (pos.x + 80), copyPos[i].y - (pos.y - 35)) + (BG::bgQuakePos() * 0.2f),
				{ redHpGauge[i], 15 },
				{ 0,0 },
				ToRadian(0),
				{ 0.9f, 0.3f, 0.0f, gaugeAlpha[i] }
			);					
			// HPゲージ
			primitive::rect(
				VECTOR2(copyPos[i].x + (pos.x + 80), copyPos[i].y - (pos.y - 35)) + (BG::bgQuakePos() * 0.2f),
				{ currentHpGauge[i], 15 },
				{ 0,0 },
				ToRadian(0),
				{ 0.0f, 0.9f, 0.5f, gaugeAlpha[i] }
			);			
			// HPゲージ
			primitive::rect(
				VECTOR2(copyPos[i].x + (pos.x + 80), copyPos[i].y - (pos.y - 45)) + (BG::bgQuakePos() * 0.2f),
				{ currentHpGauge[i], 5 },
				{ 0,0 },
				ToRadian(0),
				{ 0.0f, 0.7f, 0.3f, gaugeAlpha[i] }
			);

			//// disp Hp value
			//std::ostringstream ss;
			//ss << /*std::setw(2) << std::setfill('0') <<*/ enemys_[i]->actorComponent()->hp();
			////font::textOut(
			////	2, ss.str(),
			////	{ copyPos[i].x + pos.x + 10, copyPos[i].y - pos.y + 19 },
			////	{ 1.25f,1.25f }, { 1,1,1,1 },
			////	TEXT_ALIGN::MIDDLE_LEFT
			////);
			//

			const VECTOR2 pos2 = { 10, 165 };
			VECTOR4 color[enemyMax]{};
			{
				     if (enemys_[i]->actorComponent()->atkTimer() > enemys_[i]->actorComponent()->atkTimerMax() * 0.75f) color[i] = { 0.0f,  0.55f,  1.0f, 1 };
				else if (enemys_[i]->actorComponent()->atkTimer() > enemys_[i]->actorComponent()->atkTimerMax() * 0.55f) color[i] = { 0.0f,  0.95f, 0.0f, 1 };
				else if (enemys_[i]->actorComponent()->atkTimer() > enemys_[i]->actorComponent()->atkTimerMax() * 0.4f)  color[i] = { 1.0f, 1.0f, 0.0f, 1 };
				else if (enemys_[i]->actorComponent()->atkTimer() > enemys_[i]->actorComponent()->atkTimerMax() * 0.25f) color[i] = { 1.5f,  0.6f, 0.0f, 1 };
				else color[i] = { 1.f, 0.0f,  0.0f, 1 };
			}

			if (enemys_[i]->renderer()->sprNo() != BOSS_L_NORMAL &&
				enemys_[i]->renderer()->sprNo() != BOSS_L_FIRE   &&
				enemys_[i]->renderer()->sprNo() != BOSS_L_WATER  &&
				enemys_[i]->renderer()->sprNo() != BOSS_L_WIND   &&
				enemys_[i]->renderer()->sprNo() != BOSS_R_NORMAL &&
				enemys_[i]->renderer()->sprNo() != BOSS_R_FIRE   &&
				enemys_[i]->renderer()->sprNo() != BOSS_R_WATER  &&
				enemys_[i]->renderer()->sprNo() != BOSS_R_WIND)
			{
				// Attack
				primitive::rect(
					VECTOR2(copyPos[i].x + (pos2.x + 80), copyPos[i].y - (pos2.y - 45)) + (BG::bgQuakePos() * 0.2f),
					{ 165 * copyAtkTimer[i], 15 },
					{ 0, 0 },
					ToRadian(0),
					{ color[i].x, color[i].y, color[i].z, gaugeAlpha[i] }
				);				
				// Attack
				primitive::rect(
					VECTOR2(copyPos[i].x + (pos2.x + 80), copyPos[i].y - (pos2.y - 55)) + (BG::bgQuakePos() * 0.2f),
					{ 165 * copyAtkTimer[i], 5 },
					{ 0, 0 },
					ToRadian(0),
					{ color[i].x - 0.2f, color[i].y - 0.2f, color[i].z - 0.2f, gaugeAlpha[i] }
				);
			}
		}
	}

	// Check isPut rect
	for (i = 0; i < 4; ++i)
	{
		if (Scene::isDispHitRect_)
		{
			primitive::rect(
				enmPos[i] - VECTOR2(190, 190),
				{ 380, 380 },
				{ 0,0 }, 0, { 1, 0, 1, 0.2f }
			);
		}
	}

}

void Enemy::resetEnemy(OBJ2D* obj)
{
	obj->setExist(false);

	obj->renderer()->setSprNo(0);
	obj->transform()->setPos(VECTOR2(0, 0));
	obj->transform()->setOrgPos(VECTOR2(0, 0));
	obj->transform()->setScale(VECTOR2(1, 1));
	obj->transform()->setTexPos(VECTOR2(0, 0));
	obj->collider()->setSize(VECTOR2(0,0));
	obj->transform()->setCenter(VECTOR2(0,0));	
	obj->transform()->setAngle(ToRadian(0));
	obj->renderer()->setColor(VECTOR4(1, 1, 1, 1));
	obj->transform()->setVelocity(VECTOR2(0, 0));

	obj->collider()->setHitSize(VECTOR2(0,0));
	obj->transform()->setOffset(VECTOR2(0,0));	

	obj->renderer()->setAnimeTimer(0);
	obj->renderer()->setAnimeX(0);
	obj->renderer()->setAnimeY(0);

	obj->setState(0);
	obj->actorComponent()->setAct(0);
	obj->setElement(0);
	obj->actorComponent()->setHpMax(0);
	obj->actorComponent()->setHp(0);
	obj->actorComponent()->setAtk(0);
	obj->actorComponent()->setDmgTimer(0);
	obj->actorComponent()->setAtkTimerMax(0);
	obj->actorComponent()->setAtkTimer(0);
}

void Enemy::killAllEnm()
{
	for (auto& enm : enemys_) enm->actorComponent()->setHp(0);
}



void Enemy::enmS(OBJ2D* obj)
{
	if (!obj->exist()) return;

	const float circle_centerPosY = obj->transform()->orgPos().y;

	switch (obj->state())
	{
	case 0:
		obj->collider()->setSize(VECTOR2(380, 380));
		obj->transform()->setCenter(VECTOR2(190, 190));	
		obj->collider()->setHitSize(VECTOR2(65, 65));
		obj->transform()->setOffset(-obj->collider()->hitSize() / 2);

		obj->actorComponent()->setHpMax(ENEMY_S_HP);
		obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
		obj->actorComponent()->setAtk(1);
		
		switch (obj->renderer()->sprNo())
		{
		case ENEMY_S_NORMAL: obj->setElement(NORMAL); break;
		case ENEMY_S_FIRE:   obj->setElement(FIRE);   break;
		case ENEMY_S_WATER:  obj->setElement(WATER);  break;
		case ENEMY_S_WIND:   obj->setElement(WIND);   break;
		}

		obj->actorComponent()->setAtkTimerMax(ENEMY_S_ATK_TIMER);
		obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());
		
		obj->transform()->setVelocity(VECTOR2(0, 0.3f));

		music::play(SE_ENM_SPAWN, false);

		obj->setState(1);
		break;
	case 1:
		switch (obj->actorComponent()->act())
		{
		case 0: // entry
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 15);

			// idle
			if (obj->renderer()->animeX() >= 5)
			{
				obj->renderer()->setAnimeX(5);

				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 1: // idle
			if (obj->transform()->scale().x > 1)
			{
				obj->transform()->addScale(VECTOR2(-0.025f, -0.025f));
				if (obj->transform()->scale().x < 1) obj->transform()->setScale(VECTOR2(1, 1));
			}

			obj->transform()->addVelocityY((circle_centerPosY - obj->transform()->pos().y) / 256);
			obj->transform()->addPos(obj->transform()->velocity());

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 10);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(3);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			if (obj->actorComponent()->atkTimer() <= 0)
			{
				// attack
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(2);

				music::play(SE_ENM_ATK, false);

				obj->actorComponent()->setAct(2);
				break;
			}

			break;
		case 2: // attack
			obj->transform()->addVelocityY((circle_centerPosY - obj->transform()->pos().y) / 64/* 256*/);
			obj->transform()->addPos(obj->transform()->velocity());
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(3);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			if (obj->renderer()->animeX() == 2) obj->transform()->addScale(VECTOR2(0.05f,0.05f));
			if (obj->renderer()->animeX() >= 3)
			{
				obj->renderer()->setAnimeX(3);

				if (Game::gameHp() > 0)
				{
					Game::addGameHp(-1);
					BG::setWallQuakeState(0);
					BG::setIsWallQuake(true);
					music::play(SE_RMPT_DMG, false);
				}

				obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 3: // died
			if (obj->transform()->scale().x > 1)
			{
				obj->transform()->addScale(VECTOR2(-0.05f, -0.05f));
				if (obj->transform()->scale().x < 1) obj->transform()->setScale(VECTOR2(1, 1));
			}

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12);

			if (obj->renderer()->animeX() >= 3)
			{
				obj->renderer()->setAnimeX(3); // 一瞬画像が消えるのを防止

				obj->setExist(false);
				break;
			}

			break;
		}

		obj->transform()->setTexPos(
			VECTOR2(obj->collider()->size().x * obj->renderer()->animeX(),
					obj->collider()->size().y * obj->renderer()->animeY()));
		obj->renderer()->addAnimeTimer(1);
		break;
	}
}



void Enemy::enmM(OBJ2D* obj)
{
	if (!obj->exist()) return;

	switch (obj->state())
	{
	case 0:
		obj->collider()->setSize(VECTOR2(380, 380));
		obj->transform()->setCenter(VECTOR2(190, 190));
		obj->collider()->setHitSize(VECTOR2(5, 5));
		obj->transform()->setOffset(-obj->collider()->hitSize() / 2);

		obj->actorComponent()->setHpMax(ENEMY_M_HP);
		obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
		obj->actorComponent()->setAtk(1);

		switch (obj->renderer()->sprNo())
		{
		case ENEMY_M_NORMAL: obj->setElement(NORMAL); break;
		case ENEMY_M_FIRE:   obj->setElement(FIRE);   break;
		case ENEMY_M_WATER:  obj->setElement(WATER);  break;
		case ENEMY_M_WIND:   obj->setElement(WIND);   break;
		}

		obj->actorComponent()->setAtkTimerMax(ENEMY_M_ATK_TIMER);
		obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());

		music::play(SE_ENM_SPAWN, false);

		obj->setState(1);
		break;
	case 1:
		switch (obj->actorComponent()->act())
		{
		case 0: // entry
			// 敵同士が重なっていたらすぐmove
			for (auto& enm : enemys_)
			{
				if (obj == enm) continue;
				if (isPutEnm[0] && isPutEnm[1] && isPutEnm[2] && isPutEnm[3]) continue;
				if (Judge::rectHitCheck(
					obj->transform()->pos() + obj->transform()->offset(), obj->collider()->hitSize(),
					enm->transform()->pos() + enm->transform()->offset(), enm->collider()->hitSize()))
				{
					// move
					obj->renderer()->setAnimeTimer(-10);
					obj->renderer()->setAnimeX(-1);
					obj->renderer()->setAnimeY(0);
					obj->actorComponent()->setAct(2);
					break;
				}
			}

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 15);

			if (obj->renderer()->animeX() >= 4)
			{
				obj->renderer()->setAnimeX(5);

				// idle
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 1: // idle
			if (obj->transform()->pos().y > obj->transform()->orgPos().y)
			{
				obj->transform()->addPosY(-10);
				if (obj->transform()->pos().y < obj->transform()->orgPos().y)
				{
					obj->transform()->setPosY(obj->transform()->orgPos().y);
				}
			}
			if (obj->transform()->scale().x > 1)
			{
				obj->transform()->addScale(VECTOR2(-0.05f, -0.05f));
				if (obj->transform()->scale().x < 1) obj->transform()->setScale(VECTOR2(1, 1));
			}

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 6);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(3);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(4);
				break;
			}

			if (obj->actorComponent()->atkTimer() <= 0)
			{
				// attack
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(2);

				music::play(SE_ENM_ATK, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			// hp 1/2
			if (obj->actorComponent()->hp() <= obj->actorComponent()->hpMax() / 2)
			{
				if (isPutEnm[0] && isPutEnm[1] && isPutEnm[2] && isPutEnm[3]) break;
				if (rand() % 250 != 0) break;

				music::play(SE_ENM_SPAWN, false);

				// move
				obj->renderer()->setAnimeTimer(40);
				obj->renderer()->setAnimeX(4);
				obj->renderer()->setAnimeY(0);
				obj->actorComponent()->setAct(2);
				break;
			}

			break;
		case 2: // move
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10);

			if (obj->renderer()->animeX() <= -2)
			{
				// entry
				obj->renderer()->setAnimeX(-2);

				int randPos = rand() % 4;
				if (isPutEnm[randPos] == true) break;

				obj->transform()->setPos(enmPos[randPos]);
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(-1);
				obj->renderer()->setAnimeY(0);

				music::play(SE_ENM_SPAWN, false);

				obj->actorComponent()->setAct(0);
				break;
			}

			obj->renderer()->addAnimeTimer(-1);
			break;
		case 3: // attack
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(3);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(4);
				break;
			}

			if (obj->renderer()->animeX() == 2)
			{
				obj->transform()->addPosY(6);
				obj->transform()->addScale(VECTOR2(0.025f, 0.025f));
			} 
			if(obj->renderer()->animeX() >= 4)
			{
				// idle
				obj->renderer()->setAnimeX(4);

				if (Game::gameHp() > 0)
				{
					Game::addGameHp(-1);
					BG::setWallQuakeState(0);
					BG::setIsWallQuake(true);
					music::play(SE_RMPT_DMG, false);
				}

				obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->renderer()->setAnimeTimer(0);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 4: // died
			if (obj->transform()->pos().y > obj->transform()->orgPos().y)
			{
				obj->transform()->addPosY(-10);
				if (obj->transform()->pos().y < obj->transform()->orgPos().y)
				{
					obj->transform()->setPosY(obj->transform()->orgPos().y);
				}
			}
			if (obj->transform()->scale().x > 1)
			{
				obj->transform()->addScale(VECTOR2(-0.05f, -0.05f));
				if (obj->transform()->scale().x < 1) obj->transform()->setScale(VECTOR2(1, 1));
			}

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12);

			if (obj->renderer()->animeX() >= 3)
			{
				obj->renderer()->setAnimeX(3); // 一瞬画像が消えるのを防止

				obj->setExist(false);
				break;
			}
			break;
		}

		obj->transform()->setTexPos(
			VECTOR2(obj->collider()->size().x * obj->renderer()->animeX(),
					obj->collider()->size().y * obj->renderer()->animeY()));
		if(obj->actorComponent()->act() != 2) obj->renderer()->addAnimeTimer(1);
		break;
	}
}



void Enemy::bossLR(OBJ2D* obj)
{
	if (!obj->exist()) return;

	switch (obj->renderer()->sprNo())
	{
	case BOSS_L_NORMAL: obj->setElement(NORMAL); break;
	case BOSS_L_FIRE: obj->setElement(FIRE);   break;
	case BOSS_L_WATER: obj->setElement(WATER);  break;
	case BOSS_L_WIND: obj->setElement(WIND);   break;

	case BOSS_R_NORMAL: obj->setElement(NORMAL); break;
	case BOSS_R_FIRE: obj->setElement(FIRE);   break;
	case BOSS_R_WATER: obj->setElement(WATER);  break;
	case BOSS_R_WIND: obj->setElement(WIND);   break;
	}

	switch (obj->state())
	{
	case 0:
		obj->collider()->setSize(VECTOR2(480, 440));
		obj->transform()->setCenter(VECTOR2(240, 220));
		obj->collider()->setHitSize(VECTOR2(5, 5));
		obj->transform()->setOffset(-obj->collider()->hitSize() / 2);
		obj->renderer()->setColor(VECTOR4(0,0,0,0));

		obj->actorComponent()->setHpMax(BOSS_LR_HP);
		obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
		obj->actorComponent()->setAtk(0);

		//obj->actorComponent()->setAtkTimerMax(enmAtkTimer);
		//obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());

		obj->setTimer(0);

		obj->setState(1);
		break;
	case 1:

		switch (obj->actorComponent()->act())
		{
		case 0: // entry
			obj->renderer()->addColor(VECTOR4(0.0075f, 0.0075f, 0.0075f, 0.0075f));

			if (obj->renderer()->color().w >= 1.0f)
			{
				// idle
				obj->renderer()->setColor(VECTOR4(1,1,1,1));

				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 1: // idle
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12 % 4);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				++bossLRNum_;
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			break;
		case 2: // attack

			break;
		case 3: // died

			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12);
			obj->addTimer(1);

			if (obj->renderer()->animeX() >= 3)
			{
				obj->renderer()->setAnimeX(3);

				if (killAllEnm_) obj->setExist(false);

				obj->renderer()->setColor(VECTOR4(1, 1, 1, 1));
				if (bossLRNum_ == 2) break;

				if(obj->timer() >= 23 * 60  && obj->timer() & 0x08) obj->renderer()->setColor(VECTOR4(1.7f, 1.7f, 1.7f, 1));
				else if (obj->timer() >= 13 * 60 && 
						 obj->timer() <  23 * 60 && obj->timer() & 0x20) obj->renderer()->setColor(VECTOR4(1.65f, 1.65f, 1.65f, 1));

				if (obj->timer() >= 30 * 60 || bossLRNum_ == 0)
				{
					// revive
					for (auto& enm : enemys_)
					{
						if ((enm->renderer()->sprNo() == BOSS_LL_NORMAL ||
							enm->renderer()->sprNo()  == BOSS_LL_FIRE   ||
							enm->renderer()->sprNo()  == BOSS_LL_WATER  ||
							enm->renderer()->sprNo()  == BOSS_LL_WIND   ||
							enm->renderer()->sprNo()  == BOSS_RR_NORMAL ||
							enm->renderer()->sprNo()  == BOSS_RR_FIRE   ||
							enm->renderer()->sprNo()  == BOSS_RR_WATER  ||
							enm->renderer()->sprNo()  == BOSS_RR_WIND ) &&
							enm->actorComponent()->hp() <= 0)
						{
							enm->setTimer(-99);
						}
					}
					obj->renderer()->setColor(VECTOR4(1,1,1,1));
					obj->actorComponent()->setHpMax(ELM_BOSS_LLRR_HP);
					obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
					bossLRNum_ = 0;
					obj->setTimer(0);
					obj->renderer()->setAnimeTimer(30);
					obj->renderer()->setAnimeX(3);
					obj->renderer()->setAnimeY(1);
					obj->actorComponent()->setAct(4);
					break;
				}
			}

			break;
		case 4: // revive
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 20);

			if (obj->renderer()->animeX() <= 0)
			{
				const int bossL_sprites[] = {
					BOSS_L_FIRE,
					BOSS_L_WATER,
					BOSS_L_WIND,
				};
				const int bossR_sprites[] = {
					BOSS_R_FIRE,
					BOSS_R_WATER,
					BOSS_R_WIND,
				};

				if (obj->renderer()->sprNo() == BOSS_L_NORMAL ||
					obj->renderer()->sprNo() == BOSS_L_FIRE   ||
					obj->renderer()->sprNo() == BOSS_L_WATER  ||
					obj->renderer()->sprNo() == BOSS_L_WIND) 
					obj->renderer()->setSprNo(bossL_sprites[rand() % 3]);
				if (obj->renderer()->sprNo() == BOSS_R_NORMAL ||
					obj->renderer()->sprNo() == BOSS_R_FIRE   ||
					obj->renderer()->sprNo() == BOSS_R_WATER  ||
					obj->renderer()->sprNo() == BOSS_R_WIND) 
					obj->renderer()->setSprNo(bossR_sprites[rand() % 3]);

				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->renderer()->setAnimeTimer(0);
				obj->actorComponent()->setAct(1);
				break;
			}

			obj->renderer()->addAnimeTimer(-1);
			break;
		}

		obj->transform()->addPos(-moveBoss_);
		// 茎と根っこのズレを直す
		if (moveBoss_.y == 0 && obj->transform()->pos().y < -230) obj->transform()->setPosY(-230);
		if (moveBoss_.y == 0 && obj->transform()->pos().y > 210) obj->transform()->setPosY(210);

		obj->transform()->setTexPos(
			VECTOR2(obj->collider()->size().x * obj->renderer()->animeX(),
					obj->collider()->size().y * obj->renderer()->animeY()));
		if(obj->actorComponent()->act() != 4) obj->renderer()->addAnimeTimer(1);

		//debug::setString("obj->pos:%f", obj->transform()->pos().y);
		break;
	}
}



void Enemy::bossLLRR(OBJ2D* obj)
{
	if (!obj->exist()) return;

	switch (obj->renderer()->sprNo())
	{
	case BOSS_LL_NORMAL: obj->setElement(NORMAL); break;
	case BOSS_LL_FIRE: obj->setElement(FIRE);   break;
	case BOSS_LL_WATER: obj->setElement(WATER);  break;
	case BOSS_LL_WIND: obj->setElement(WIND);   break;

	case BOSS_RR_NORMAL: obj->setElement(NORMAL); break;
	case BOSS_RR_FIRE: obj->setElement(FIRE);   break;
	case BOSS_RR_WATER: obj->setElement(WATER);  break;
	case BOSS_RR_WIND: obj->setElement(WIND);   break;
	}

	switch (obj->state())
	{
	case 0:
		obj->collider()->setSize(VECTOR2(480, 440));
		obj->transform()->setCenter(VECTOR2(240, 220));
		obj->collider()->setHitSize(VECTOR2(5, 5));
		obj->transform()->setOffset(-obj->collider()->hitSize() / 2);
		obj->renderer()->setColor(VECTOR4(0, 0, 0, 0));

		obj->actorComponent()->setHpMax(BOSS_LLRR_HP);
		obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
		obj->actorComponent()->setAtk(1);

		obj->actorComponent()->setAtkTimerMax(BOSS_LLRR_ATK_TIMER);
		obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());

		music::fade(BGM_PLAY, 1.0f);

		obj->setState(1);
		break;
	case 1:

		switch (obj->actorComponent()->act())
		{
		case 0: // entry
			obj->renderer()->addColor(VECTOR4(0.0075f, 0.0075f, 0.0075f, 0.0075f));

			if (obj->renderer()->color().w >= 1.0f)
			{
				obj->renderer()->setColor(VECTOR4(1, 1, 1, 1));

				// idle
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->renderer()->setAnimeY(0);

				music::play(BGM_BOSS, true);

				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 1: // idle
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 4);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(2);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			if (obj->actorComponent()->atkTimer() <= 0)
			{
				// attack
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				music::play(SE_ENM_ATK, false);
				obj->actorComponent()->setAct(2);
				break;
			}

			break;
		case 2: // attack
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(2);

				music::play(SE_ENM_DEATH, false);

				obj->actorComponent()->setAct(3);
				break;
			}

			//if (obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_LL_NORMAL ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_LL_FIRE   ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_LL_WATER  ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_LL_WIND   ) obj->transform()->addPosX(7);
			//if (obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_RR_NORMAL ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_RR_FIRE   ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_RR_WATER  ||
			//	obj->renderer()->animeX() == 0 && obj->renderer()->sprNo() == BOSS_RR_WIND   ) obj->transform()->addPosX(-7);

			//if (obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_LL_NORMAL ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_LL_FIRE   ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_LL_WATER  ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_LL_WIND   ) obj->transform()->addPosX(-7);
			//if (obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_RR_NORMAL ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_RR_FIRE   ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_RR_WATER  ||
			//	obj->renderer()->animeX() == 1 && obj->renderer()->sprNo() == BOSS_RR_WIND   ) obj->transform()->addPosX(7);

			//if (obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_LL_NORMAL ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_LL_FIRE   ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_LL_WATER  ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_LL_WIND   ) obj->transform()->addPosX(7);
			//if (obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_RR_NORMAL ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_RR_FIRE   ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_RR_WATER  ||
			//	obj->renderer()->animeX() == 2 && obj->renderer()->sprNo() == BOSS_RR_WIND   ) obj->transform()->addPosX(-7);

			if (obj->renderer()->animeX() >= 3)
			{
				// idle
				obj->renderer()->setAnimeX(3);

				if (Game::gameHp() > 0)
				{
					Game::addGameHp(-1);
					BG::setWallQuakeState(0);
					BG::setIsWallQuake(true);
					music::play(SE_RMPT_DMG, false);
				}


				obj->transform()->setPos(obj->transform()->orgPos());
				obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->renderer()->setAnimeTimer(0);
				obj->actorComponent()->setAct(1);
				break;
			}

			break;
		case 3: // died
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12);

			if (obj->renderer()->animeX() >= 2)
			{
				obj->renderer()->setAnimeX(2);

				if (killAllEnm_) obj->setExist(false);

				if (bossLRNum_ == 2)
				{
					obj->setTimer(-99);
					break;
				}
				if (obj->timer() == -99 && bossLRNum_ == 0)
				{
					// revive
					obj->setTimer(0);
					obj->actorComponent()->setHpMax(ELM_BOSS_LR_HP);
					obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
					obj->renderer()->setAnimeTimer(24);
					obj->renderer()->setAnimeX(2);
					obj->renderer()->setAnimeY(2);
					obj->actorComponent()->setAct(4);
					break;
				}
			}

			break;
		case 4: // revive
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12);

			if (obj->renderer()->animeX() <= 0)
			{
				const int bossLL_sprites[] = {
					BOSS_LL_FIRE,
					BOSS_LL_WATER,
					BOSS_LL_WIND,
				};				
				const int bossRR_sprites[] = {
					BOSS_RR_FIRE,
					BOSS_RR_WATER,
					BOSS_RR_WIND,
				};

				if (obj->renderer()->sprNo() == BOSS_LL_NORMAL ||
					obj->renderer()->sprNo() == BOSS_LL_FIRE   ||
					obj->renderer()->sprNo() == BOSS_LL_WATER  ||
					obj->renderer()->sprNo() == BOSS_LL_WIND)
					obj->renderer()->setSprNo(bossLL_sprites[rand() % 3]);
				if (obj->renderer()->sprNo() == BOSS_RR_NORMAL ||
					obj->renderer()->sprNo() == BOSS_RR_FIRE   ||
					obj->renderer()->sprNo() == BOSS_RR_WATER  ||
					obj->renderer()->sprNo() == BOSS_RR_WIND   ) 
					obj->renderer()->setSprNo(bossRR_sprites[rand() % 3]);

				obj->actorComponent()->setAtkTimer(obj->actorComponent()->atkTimerMax());

				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->actorComponent()->setAct(1);
				break;
			}

			obj->renderer()->addAnimeTimer(-1);
			break;
		}

		obj->transform()->addPos(-moveBoss_);
		// 茎と根っこのズレを直す
		if (moveBoss_.y == 0 && obj->transform()->pos().y < -230 + 1) obj->transform()->setPosY(-230 + 1);
		if (moveBoss_.y == 0 && obj->transform()->pos().y > 210 + 1)
		{
			if(obj->actorComponent()->act() != 2) obj->transform()->setPosY(210 + 1);
		}

		obj->transform()->setTexPos(
			VECTOR2(obj->collider()->size().x * obj->renderer()->animeX(),
					obj->collider()->size().y * obj->renderer()->animeY()));
		if (obj->actorComponent()->act() != 4) obj->renderer()->addAnimeTimer(1);
		break;
	}
}

void Enemy::bossCenter(OBJ2D* obj)
{
	if (!obj->exist()) return;

	static int emergingTime = 0; // 地上にいる時間

	switch (obj->state())
	{
	case 0:
		obj->collider()->setSize(VECTOR2(1920, 440));
		obj->transform()->setTexPos(VECTOR2(0, -obj->collider()->size().y));
		obj->transform()->setCenter(VECTOR2(960, 220));
		obj->collider()->setHitSize(VECTOR2(350, 30));
		obj->transform()->setOffset(-obj->collider()->hitSize() / 2);

		obj->actorComponent()->setHpMax(BOSS_CENTER_HP);
		obj->actorComponent()->setHp(obj->actorComponent()->hpMax());
		obj->actorComponent()->setAtk(0);

		obj->setElement(NORMAL);
		obj->setTimer(0);

		emergingTime = 10 * 60; // 最初は10秒


		obj->setState(1);
		break;
	case 1:

		switch (obj->actorComponent()->act())
		{
		case 0: // underGround
			if (bossLRNum_ == 2)
			{
				moveBoss_ = { 0, 3 };

				music::play(SE_BOSS_QUAKE, false);

				BG::setBgQuakeNum(BOSS_QUAKE);
				BG::setBgQuakeState(0);
				BG::setIsBgQuake(true);
				obj->actorComponent()->setAct(1);
				break;
			}
			break;
		case 1: // entry
			obj->transform()->addTexPosY(moveBoss_.y);
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 2);

			if (obj->transform()->texPos().y >= 0)
			{
				moveBoss_ = {};
				obj->transform()->setTexPos(VECTOR2(0,0));


				// idle
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->renderer()->setSprNo(BOSS_CENTER);
				obj->actorComponent()->setAct(2);
				break;
			}

			obj->transform()->setTexPosX(obj->collider()->size().x * obj->renderer()->animeX());
			break;
		case 2: // idle hp 61 ~ 100%
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12 % 6);

			// hp 41 ~ 60%
			if (obj->actorComponent()->hp() <= (obj->actorComponent()->hpMax() * 0.6f))
			{
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(1);
				obj->actorComponent()->setAct(3);
				break;
			}		
			break;
		case 3: // hp 41 ~ 60%
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 12 % 6);

			// hp 21 ~ 40%
			if (obj->actorComponent()->hp() <= (obj->actorComponent()->hpMax() * 0.4f))
			{
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(2);
				obj->actorComponent()->setAct(4);
				break;
			}
			break;
		case 4: // hp 21 ~ 40%
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 4);

			// hp 1 ~ 20%
			if (obj->actorComponent()->hp() <= (obj->actorComponent()->hpMax() * 0.2f))
			{
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(3);
				obj->actorComponent()->setAct(5);
				break;
			}
			break;
		case 5: // hp 1 ~ 20%
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 2);

			if (obj->actorComponent()->hp() <= 0)
			{
				// died
				killAllEnm_ = true;

				music::play(SE_BOSS_DEATH, false);

				BG::setBgQuakeNum(BOSS_DIED_QUAKE);
				BG::setBgQuakeState(0);
				BG::setIsBgQuake(true);

				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(4);
				obj->actorComponent()->setAct(7);
				break;
			}

			break;
		case 6: // move
			obj->transform()->addTexPosY(moveBoss_.y);
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 10 % 2);

			if (obj->transform()->texPos().y <= -obj->collider()->size().y)
			{
				// underGround
				obj->transform()->setTexPos(VECTOR2(0, -obj->collider()->size().y));
				moveBoss_ = {};
				bossLRNum_ = 0;
				obj->renderer()->setAnimeTimer(0);
				obj->renderer()->setAnimeX(0);
				obj->renderer()->setAnimeY(0);
				obj->actorComponent()->setAct(0);
				break;
			}

			obj->transform()->setTexPosX(obj->collider()->size().x * obj->renderer()->animeX());
			break;

			break;
		case 7: // died
			obj->renderer()->setAnimeX(obj->renderer()->animeTimer() / 90);

			music::fade(BGM_BOSS, 2.0f, 0.0f);

			if (obj->renderer()->animeX() >= 6)
			{
				obj->renderer()->setAnimeX(6);

				bossLRNum_ = 0;
				obj->setExist(false);

				music::resume(BGM_PLAY);
				break;
			}
			break;
		}


		if (obj->timer() >= emergingTime)
		{
			// move
			obj->setTimer(0);
			emergingTime = 20 * 60; // 2回目以降は20秒
			moveBoss_ = { 0, -3 };
			obj->renderer()->setSprNo(BOSS_CENTER_ENTRY);
			obj->transform()->setTexPos(VECTOR2(0,0));
			obj->renderer()->setAnimeTimer(0);
			obj->renderer()->setAnimeX(0);
			obj->renderer()->setAnimeY(0);

			music::play(SE_BOSS_QUAKE, false);

			BG::setBgQuakeNum(BOSS_QUAKE);
			BG::setBgQuakeState(0);
			BG::setIsBgQuake(true);
			obj->actorComponent()->setAct(6);
			break;
		}

		//debug::setString("obj->texPos:%f", obj->transform()->texPos().y);


		if (obj->actorComponent()->act() != 0 &&
			obj->actorComponent()->act() != 1 &&
			obj->actorComponent()->act() != 6)
		{
			obj->transform()->setTexPos(
				VECTOR2(obj->collider()->size().x * obj->renderer()->animeX(),
						obj->collider()->size().y * obj->renderer()->animeY()));
		}
		obj->renderer()->addAnimeTimer(1);

		if (obj->actorComponent()->act() != 0 &&
			obj->actorComponent()->act() != 1 &&
			obj->actorComponent()->act() != 6 &&
			obj->actorComponent()->act() != 7) obj->addTimer(1);
		break;
	}
}
