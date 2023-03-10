#include "all.h"
#include <sstream>
#include <iomanip>

int   ui_state;
int   cost;

int   time_limit;

float feverFramePosX_1, feverFramePosX_2;
float slimeWaveTextPosX;

BOOL  feverAlready;
BOOL  feverMode;

OBJ2D   hp;
OBJ2D   element;
OBJ2D   feverGauge;
OBJ2D   feverText;
OBJ2D   feverButton;
//OBJ2D   pause;

Sprite* sprHp;
Sprite* sprElemental;

Sprite* sprSlimeWave;

void ui_init() {
	ui_state = 0;
}

void ui_deinit() {
	safe_delete(sprHp);
	safe_delete(sprElemental);

	safe_delete(sprSlimeWave);
}

void ui_update() {

	switch (ui_state) {

	case 0:

		sprHp         = sprite_load(L"./Data/Images/HP.png");
		sprElemental  = sprite_load(L"./Data/Images/Elemental.png");

		sprSlimeWave  = sprite_load(L"./Data/Images/SlimeWave.png");

		++ui_state;
		/*fallthrough*/
	case 1:

		time_limit = 7260;
		if (curScene == SCENE_TUTORIAL) time_limit = 7200;

		cost = -1;
		if (curScene == SCENE_TUTORIAL) cost = 1;

		feverAlready = FALSE;
		feverMode = FALSE;

		feverFramePosX_1 = 0;
		feverFramePosX_2 = 9600;

		slimeWaveTextPosX = -SCREEN_W - 250;


		hp.hp      = HP_HP;
		hp.pos     = { HP_POS_X,      HP_POS_Y };
		hp.scale   = { HP_SCALE,      HP_SCALE };
		hp.texSize = { HP_TEX_SIZE_X, HP_TEX_SIZE_Y };
		hp.texPos  = { 0, hp.hp * hp.texSize.y };
		hp.color   = VEC4_WHITE;

		element.pos     = { ELM_POS_X, ELM_POS_Y };
		element.scale   = { ELM_SCALE, ELM_SCALE };
		element.texSize = { ELM_TEX_SIZE_X, ELM_TEX_SIZE_Y };
		element.texPos  = { 0, cost * element.texSize.y };
		element.color   = VEC4_WHITE;
		element.timer   = 0;

		feverGauge.anime      = 0;
		feverGauge.animeTimer = 0;
		feverGauge.animeY     = 0;
		feverGauge.timer      = 0;
		feverGauge.pos        = { 990, -10 };
		feverGauge.scale      = { 1.0f, 1.0f };
		feverGauge.texPos     = { 0, 0 };
		feverGauge.texSize    = { 900, 200 };
		feverGauge.color      = VEC4_WHITE;
		feverGauge.timer      = 0;

		feverButton.pos     = { feverGauge.pos.x + 35, feverGauge.pos.y + 35 };
		feverButton.texSize = { feverGauge.texSize.x - 30, feverGauge.texSize.y - 70};

		feverText.anime      = 0;
		feverText.animeTimer = 0;
		feverText.pos        = { 0, 0 };
		feverText.scale      = { 1.0f, 1.0f };
		feverText.texPos     = { 0, feverText.anime * feverText.texSize.y };
		feverText.texSize    = { SCREEN_W, SCREEN_H };
		feverText.color      = { 1,1,1,0.6f };
		feverText.timer      = 0;

		//pause.pos = {SCREEN_W_HALF, 0};
		//pause.scale = { 1.0f, 1.0f };
		//pause.texSize = {50, 50};

		++ui_state;
		/*fallthrough*/
	case 2:

		feverFramePosX_1 -= 50.0f;
		feverFramePosX_2 -= 50.0f;

		if (feverFramePosX_1 <= -9600.0f) feverFramePosX_1 += 9600.0f * 2;
		if (feverFramePosX_2 <= -9600.0f) feverFramePosX_2 += 9600.0f * 2;

		//if (time_limit == 90 * 60)      slimeWaveTextPosX = SCREEN_W + 200;
		//else if (time_limit == 30 * 60) slimeWaveTextPosX = SCREEN_W + 200;

		// スライムウェーブテキストの移動
		if (time_limit == 90 * 60)      slimeWaveTextPosX = SCREEN_W + 200;
		if (time_limit == 30 * 60)      slimeWaveTextPosX = SCREEN_W + 200;

		if(slimeWaveTextPosX > -SCREEN_W - 1200) slimeWaveTextPosX -= 20.0f;


		// TODO:敵出現時間設定
		if (difficulty_easy)
		{
			     if (time_limit < 10 * 60) coolTime = 180; // 3
			else if (time_limit < 30 * 60) coolTime = 60;  // 1
			else if (time_limit < 60 * 60) coolTime = 180; // 3
			else if (time_limit < 90 * 60) coolTime = 60;  // 1
		}		
		else if (difficulty_normal)
		{
			     if (time_limit < 10 * 60) coolTime = 120;	// 2
			else if (time_limit < 30 * 60) coolTime = 50;	// 0.9?
			else if (time_limit < 60 * 60) coolTime = 120;	// 2
			else if (time_limit < 90 * 60) coolTime = 60;	// 1
		}		
		else if (difficulty_hard)
		{
			     if (time_limit < 5 * 60)  coolTime = 60; // 1
			else if (time_limit < 30 * 60) coolTime = 10; // 0.1
			else if (time_limit < 60 * 60) coolTime = 60; // 1
			else if (time_limit < 90 * 60) coolTime = 30; // 0.5
		}

		//if (time_limit == 10 * 60)
		//{
		//	music::setVolume(BGM_GAME, 0.2f);
		//	music::stop(BGM_WAVE);
		//}
		if (time_limit == 25 * 60)
		{
			if (feverGauge.timer <= 0) 
			{
				music::setVolume(BGM_GAME, 0.01f);
				music::play(BGM_WAVE, TRUE);
			}
		}
		else if (time_limit == 60 * 60)
		{
			music::setVolume(BGM_GAME, 0.2f);
			music::stop(BGM_WAVE);
		}
		else if (time_limit == 85 * 60)
		{
			if (feverGauge.timer <= 0)
			{
				music::setVolume(BGM_GAME, 0.01f);
				music::play(BGM_WAVE, TRUE);
			}
		}


		//cost = 10;


		if (curScene == SCENE_GAME)
		{
			if (feverGauge.timer > 0 && game_timer % 30 == 0 && cost < 10) cost++;
			else if (game_timer % 60 == 0 && cost < 10) cost++;
		}


		if (element.timer > 0)    element.timer--;
		if (feverGauge.timer > 0) feverGauge.timer--;
		if (feverGauge.timer > 0) feverText.animeTimer++;
		if (feverText.timer > 0)  feverText.timer--;


		if (curScene == SCENE_GAME)
		{
			if (feverGauge.timer > 0 && game_timer % 10 == 0) feverGauge.anime--;
			else if (feverGauge.animeY == 9)
			{
				if (feverGauge.timer <= 0 && game_timer % 9 == 0) feverGauge.anime++;
			}
			else if (game_timer % 30 == 0) feverGauge.anime++;
		}
		if (curScene == SCENE_TUTORIAL && board_state == 6)
		{
			feverGauge.anime = 1;
			feverGauge.animeY = 9;
		}

		// フィーバーゲージのアニメ
		if (feverGauge.timer > 0)
		{
			if (feverGauge.anime <= 0 && feverGauge.animeY <= 0)
			{
				feverGauge.anime = 0;
			}
			else if (feverGauge.anime <= 0 && feverGauge.animeY > 0)
			{
				feverGauge.animeY--;
				feverGauge.anime = 9;
			}
		}
		else
		{
			if (feverGauge.anime >= 9 && feverGauge.animeY < 9)
			{
				feverAlready = FALSE;
				feverGauge.animeY++;
				feverGauge.anime = 0;
			}
			else if (feverGauge.anime >= 2 && feverGauge.animeY == 9)
			{
				feverAlready = TRUE;
				feverGauge.anime = 0;
			}
		}


		hp.texPos.y         = hp.hp * hp.texSize.y;
		element.texPos.y    = cost * element.texSize.y;
		feverGauge.texPos.x = feverGauge.anime * feverGauge.texSize.x;
		feverGauge.texPos.y = feverGauge.animeY * feverGauge.texSize.y;
		feverText.anime     = feverText.animeTimer / 5 % 4;
		feverText.texPos.x  = feverText.anime * feverText.texSize.x;

		if (showHitCheck)
		{
			debug::setString("feverTime:%d", feverGauge.timer);
		}

		if (curScene == SCENE_GAME && time_limit > 0) time_limit--;
		break;
	}
}

void ui_render() {

	if (curScene == SCENE_GAME || curScene == SCENE_TUTORIAL)
	{
		if (feverGauge.timer > 0)
		{
			if (feverText.timer <= 0 && feverText.color.w > 0.425f) feverText.color.w -= 0.05f;
			sprite_render(
				sprFeverText,
				feverText.pos.x, feverText.pos.y,
				feverText.scale.x, feverText.scale.y,
				feverText.texPos.x, feverText.texPos.y,
				feverText.texSize.x, feverText.texSize.y,
				feverText.pivot.x, feverText.pivot.y,
				feverText.angle,
				feverText.color.x, feverText.color.y,
				feverText.color.z, feverText.color.w
			);

			sprite_render(
				sprFeverFrame,
				feverFramePosX_1, 0,
				1.0f, 1.0f,
				0, 0,
				9600, 1080
			);
			sprite_render(
				sprFeverFrame,
				feverFramePosX_2, 0,
				1.0f, 1.0f,
				0, 0,
				9600, 1080
			);
		}

		if (curScene == SCENE_GAME || board_state >= 2)
		{
			if (game_timer & 0x18 && hp.hp <= 3) hp.color = { 1.0f, 0.2f, 0.2f, 1.0f };
			sprite_render(
				sprHp,
				hp.pos.x, hp.pos.y,
				hp.scale.x, hp.scale.y,
				hp.texPos.x, hp.texPos.y,
				hp.texSize.x, hp.texSize.y,
				hp.pivot.x, hp.pivot.y,
				hp.angle,
				hp.color.x, hp.color.y, hp.color.z, hp.color.w
			);
			hp.color = VEC4_WHITE;
		}

		{
			if (curScene == SCENE_GAME || board_state >= 1)
			{
				VECTOR4 elmColor = VEC4_WHITE;
				if (cost < 10 && game_timer % 60 == 0)        elmColor = { 1.27f, 1.27f, 1.27f, 1.0f };
				if (feverGauge.timer > 0 && game_timer & 0x08) elmColor = { 1.5f, 1.5f, 1.5f, 1.0f };
				if (element.timer > 0 && game_timer & 0x08)    elmColor = { 0.5f, 0.3f, 0.1f, 1.0f };
				sprite_render
				(
					sprElemental,
					element.pos.x, element.pos.y,
					element.scale.x, element.scale.y,
					element.texPos.x, element.texPos.y,
					element.texSize.x, element.texSize.y,
					element.pivot.x, element.pivot.y,
					element.angle,
					elmColor.x, elmColor.y, elmColor.z, elmColor.w
				);
				if (element.timer > 0 && game_timer & 0x08)
				{
					text_out(6, "No Cost!", 360, 1000,
						1.7f, 1.7f, 1.0f, 0.2f, 0, 1, TEXT_ALIGN::MIDDLE);
				}
			}
		}

		//feverGauge.color = VEC4_WHITE;
		//if (feverAlready && game_timer & 0x04)
		//{
		//	feverGauge.color.x = feverGauge.color.y = feverGauge.color.z = 1.3f;
		//}

		if (curScene == SCENE_GAME || board_state >= 4)
		{
			sprite_render
			(
				sprFeverGauge,
				feverGauge.pos.x, feverGauge.pos.y,
				feverGauge.scale.x, feverGauge.scale.y,
				feverGauge.texPos.x, feverGauge.texPos.y,
				feverGauge.texSize.x, feverGauge.texSize.y,
				feverGauge.pivot.x, feverGauge.pivot.y,
				feverGauge.angle,
				feverGauge.color.x, feverGauge.color.y,
				feverGauge.color.z, feverGauge.color.w
			);
		}

		if (showHitCheck)
		{
			primitive::rect(
				feverButton.pos,
				feverButton.texSize,
				feverButton.pivot,
				ToRadian(0),
				{ 0, 1, 0, 0.2f }
			);
		}


		std::ostringstream ss; //文字列ストリームを用意

		//1分より大きかったら60引く
		int minTime = 120;
		     if (time_limit >= 7200) minTime = 120;
		else if (time_limit > 3600)  minTime = 60;
		else if (time_limit >= 0)    minTime = 0;

		//８桁で空きを'０'で埋める
		ss << time_limit / 3600 << " " << std::setw(2) << std::setfill('0')
			<< (time_limit / 60) - minTime;

		VECTOR4 timeColor{ 1.2f,1.2f,1.2f,1.0f };
		     if (time_limit / 60 <= 0)                          timeColor = VEC4_WHITE;
		else if (time_limit / 60 <= 30 && time_limit / 60 > 10) timeColor = {1.0f,0.9f,0,1};
	    else if (time_limit / 60 <= 10)                         timeColor = {1.5f,0,0,1};
		else if ((time_limit / 60) % 30 == 0)                   timeColor = {1.5f,0,0,1};

		if (curScene == SCENE_GAME || board_state >= 4)
		{
			text_out(
				6,
				ss.str(),
				330, 106,
				3.0f, 3.0f,
				0.1f, 0.1f, 0.1f, 1.0f,
				TEXT_ALIGN::MIDDLE
			);
			text_out(
				6,
				":",
				305, 96,
				3.0f, 3.0f,
				0.1f, 0.1f, 0.1f, 1.0f,
				TEXT_ALIGN::MIDDLE
			);	

			text_out(
				6,
				ss.str(),
				325, 100,
				3.0f, 3.0f,
				timeColor.x, timeColor.y,
				timeColor.z, timeColor.w,
				TEXT_ALIGN::MIDDLE
			);
			text_out(
				6,
				":",
				300, 90,
				3.0f, 3.0f,
				timeColor.x, timeColor.y,
				timeColor.z, timeColor.w,
				TEXT_ALIGN::MIDDLE
			);
		}

		if (curScene == SCENE_GAME && slimeWaveTextPosX > -SCREEN_W - 1200)
		{
			sprite_render(
				sprSlimeWave,
				slimeWaveTextPosX, 250,
				3.0f, 2.0f,
				0,0,
				700, 250
			);			
		}
	}

}