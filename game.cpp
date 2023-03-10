#include "all.h"
#include "easing.h"

#include "imgui_manager.h"
#include "Font/directX11Render2D.h"
#include "Font/fontTexture.h"
#include "Font/dispString.h"
#include <tchar.h>

Game Game::instance_;

int Game::gameHp_ = 0;
int Game::copyGameHp_ = 0;

bool Game::isPlayedSE_ = false;

Game::Wave Game::wave_;

void Game::init()
{
	Scene::init();
	
	pBg_     = new BG;
	pPlayer_ = new Player;
	pEnemy_  = new Enemy;
	pItem_   = new Item;
	pEffect_ = new Effect;
}

void Game::deinit()
{
	pBg_->deinit();
	pPlayer_->deinit();
	pEnemy_->deinit();
	pItem_->deinit();
	pEffect_->deinit();

	// 各マネージャの開放
	safe_delete(pBg_);
	safe_delete(pPlayer_);
	safe_delete(pEnemy_);
	safe_delete(pItem_);
	safe_delete(pEffect_);

	texture::release(loadTextureGame);
}

void Game::update()
{
	using namespace input;

	//if (STATE(0) & PAD_LCLICK)
	//{
	//	Sleep(60);
	//}

	// dispHitRect_
	//if (TRG(0) & PAD_RCLICK)
	//{
	//	if (!isDispHitRect_) Scene::isDispHitRect_ = true;
	//	else Scene::isDispHitRect_ = false;
	//}

	switch (state_)
	{
	case 0:
		//////// 初期設定 ////////
		pBg_->init();
		pPlayer_->init();
		pEnemy_->init();
		pItem_->init();
		pEffect_->init();

		timer_ = 0;
		gameHp_ = gameHpMax_;
		copyGameHp_ = gameHp_;

		isPlayedSE_ = false;

		wave_ = { WAVE_1, {0 - 625, 200}, {1250,350}, {70, 0} };
		if (Enemy::enemySet_ == enemySets[2])      wave_.sprNo = WAVE_3;
		else if (Enemy::enemySet_ == enemySets[1]) wave_.sprNo = WAVE_2;
		else if (Enemy::enemySet_ == enemySets[0]) wave_.sprNo = WAVE_1;

		Scene::isIrisEnd_ = false; // インが終わる前にstateが進むのを防ぐ
		Scene::setIrisInOut(true);

		GameLib::setBlendMode(Blender::BS_ALPHA);   // 通常のアルファ処理
		texture::load(loadTextureGame);

		music::play(BGM_PLAY, true);


		++state_;
		[[fallthrough]];
	case 1: // IrisIn
		pBg_->update();
		pPlayer_->update();

		if (Scene::isIrisEnd_)
		{
			music::play(SE_WAVE, false);
			state_ = 5; // wave moveInOut
			break;
		}

		break;
	case 2:
		// ソフトリセット
		if (GetAsyncKeyState(VK_ESCAPE) < 0 && Scene::isIrisEnd_ && Scene::irisInOut() == true)
		{
			music::play(SE_DECIDE, false);

			Scene::isIrisEnd_ = false;
			Scene::setIrisInOut(false);
			timer_ = 0;
			state_ = 4; // Title
			break;
		}

		pBg_->update();
		pPlayer_->update();
		pEnemy_->update();
		pItem_->update();
		pEffect_->update();
		Judge::gameJudge();

		if (Enemy::enemySet_ == enemySets[2])      wave_.sprNo = WAVE_3;
		else if (Enemy::enemySet_ == enemySets[1]) wave_.sprNo = WAVE_2;
		else if (Enemy::enemySet_ == enemySets[0]) wave_.sprNo = WAVE_1;

		if (BG::isBgQuake())   BG::bgQuake();
		if (BG::isWallQuake()) BG::wallQuake();

		if (gameHp_ <= 2 && !isPlayedSE_)
		{
			music::play(SE_HP_WARNING, false);
			isPlayedSE_ = true;
		}

		++timer_;
		break;
	case 3: // gameover
		music::fade(BGM_PLAY, 0.5f, 0.0f);
		music::fade(BGM_BOSS, 0.5f, 0.0f);

		pBg_->update();
		pPlayer_->update();
		pEnemy_->update();
		pItem_->update();
		pEffect_->update();

		if (BG::isBgQuake())   BG::bgQuake();
		if (BG::isWallQuake()) BG::wallQuake();

		if (Scene::isIrisEnd_)
		{
			Scene::changeScene(Over::instance());
			return;
		}

		break;
	case 4: // title
		music::fade(BGM_PLAY, 0.5f, 0.0f);
		music::fade(BGM_BOSS, 0.5f, 0.0f);

		pBg_->update();
		pPlayer_->update();
		pEnemy_->update();
		pItem_->update();
		pEffect_->update();
		Judge::gameJudge();

		if (BG::isBgQuake())   BG::bgQuake();
		if (BG::isWallQuake()) BG::wallQuake();

		if (Scene::isIrisEnd_)
		{
			++Title::skyColorNum_;
			if (Title::skyColorNum_ > 2) Title::skyColorNum_ = 0;

			Scene::changeScene(Title::instance());
			return;
		}

		break;
	case 5: // wave moveInOut
		pBg_->update();
		pPlayer_->update();
		pItem_->update();
		pEffect_->update();
		Judge::gameJudge();

		if (BG::isBgQuake())   BG::bgQuake();
		if (BG::isWallQuake()) BG::wallQuake();

		if (moveInOutRight(&wave_.pos, &wave_.velocity))
		{
			wave_.pos	   = { 0 - 625, 200 };
			wave_.velocity = { 70, 0 };
			state_ = 2;
			break;
		}

		break;

	case 6: // gameClear
		pBg_->update();
		pPlayer_->update();
		pEnemy_->update();
		pItem_->update();
		pEffect_->update();
		Judge::gameJudge();

		if (BG::isBgQuake())   BG::bgQuake();
		if (BG::isWallQuake()) BG::wallQuake();

		if (Scene::isIrisEnd_)
		{
			Scene::changeScene(Clear::instance());
			return;
		}
	}
}

void Game::draw()
{
	GameLib::clear(VECTOR4(0.36f, 0.25f, 0.26f, 1));

	pBg_->render();
	pPlayer_->render();
	// RabbitStar
	//for (int i = 0; i < Item::itemMax; ++i)
	//{
	//	if (rabbitStar[i].isDisp)
	//	{
	//		texture::begin(RABBIT_STAR);
	//		texture::draw(
	//			RABBIT_STAR,
	//			{ Item::items_[i]->transform()->pos().x, 190 },
	//			{ 1,1 },
	//			rabbitStar[i].texPos,
	//			{150, 150},
	//			{ 75, 75 },
	//			0,
	//			{1,1,1,1}
	//			);
	//		texture::end(RABBIT_STAR);
	//	}
	//}
	pEnemy_->render();

	texture::begin(ELEMENT);
	texture::draw(
		ELEMENT,
		VECTOR2(110,100)/*VECTOR2(1810, 980)*/ + (BG::bgQuakePos() * 0.2f), { 1,1 },
		{ 0,0 }, { 200,200 },
		{ 100,100 }, ToRadian(0),
		{ 1,1,1,0.7f }
	);
	texture::end(ELEMENT);

	pItem_->render();
	pEffect_->render();

	texture::begin(wave_.sprNo);
	texture::draw(
		wave_.sprNo,
		wave_.pos + (BG::bgQuakePos() * 0.2f), { 1,1 },
		{ 0,0 }, wave_.size,
		wave_.size / 2, ToRadian(0),
		{ 1,1,1,1 }
	);
	texture::end(wave_.sprNo);



	// ボス撃破時の白点滅
	static float colorAlpha = 0;
	for (auto& enm : Enemy::enemys_)
	{
		if (enm->renderer()->sprNo() == BOSS_CENTER)
		{
			if (state_ == 0) colorAlpha = 0;

			if (enm->actorComponent()->hp() <= 0)
			{
				if (enm->renderer()->animeTimer() == 60)  colorAlpha = 0.8f;
				if (enm->renderer()->animeTimer() == 100) colorAlpha = 0.8f;
				if (enm->renderer()->animeTimer() == 130) colorAlpha = 0.8f;
			}
		}			 
	}
	if (colorAlpha > 0)
	{
		colorAlpha -= 0.02f;
	}
	primitive::rect(
		{ 0,0 }, { 1920, 1080 },
		{ 0,0 }, ToRadian(0), 
		{1,1,1,colorAlpha}
	);


	//float speedX = Elastic::easeIn(timer_, 0, 1, 10);
	//posX += speedX;
	//posX = Circ::easeOut(timer_, 0, 1, 600);

	//primitive::rect(
	//	0, BG::WINDOW_H / 3.5f + posX,
	//	10, 10,
	//	0,0, 0, 0,0,0,1.0f);

	//const float size2 = 48;
	//DispString::instance().dispString(_T("魑魅魍魎　：；。、？！ー＝"),			0, size2 * 0, size2, size2, size2, { 1, 0, 1, 1 });
	//DispString::instance().dispString(_T("あかさたなはまやらわやあああああああああああああああああああ"), 0, size2 * 1, size2, size2, size2, { 1, 1, 1, 1 });
	//DispString::instance().dispString(_T("ニワトリニワニワトリ"),			0, size2 * 2, size2, size2, size2, { 1, 0, 0, 1 });
	//DispString::instance().dispString(_T("DO　DO"),				0, size2 * 3, size2, size2, size2, { 0, 0, 0, 1 });
}

void Game::reset()
{
	state_ = 0;
}