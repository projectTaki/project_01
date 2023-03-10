#include "all.h"

int     shot_state;
OBJ2D	plShot[SHOT_MAX];

Sprite* sprFlameShot;
Sprite* sprWaterShot;
Sprite* sprEarthShot;


void shot_init() 
{
	shot_state = 0;
}

void shot_deinit() 
{
	safe_delete(sprFlameShot);
	safe_delete(sprWaterShot);
	safe_delete(sprEarthShot);
}

void shot_update() 
{

	switch (shot_state)
	{
	case 0:
		//////// ‰Šúİ’è ////////
		sprFlameShot = sprite_load(L"./Data/Images/FlameShot.png");
		sprWaterShot = sprite_load(L"./Data/Images/WaterShot.png");
		sprEarthShot = sprite_load(L"./Data/Images/EarthShot.png");

		++shot_state;
		/*fallthrough*/

	case 1:
		//////// ƒpƒ‰ƒ[ƒ^‚Ìİ’è ////////

		int i;

		for (i = 0; i < SHOT_MAX; ++i)
		{
			plShot[i].exist = FALSE;
		}

		++shot_state;
		/*fallthrough*/
	case 2:
		for (i = 0; i < SHOT_MAX; i++)
		{
			if (plShot[i].exist == FALSE) continue;
			
			switch (plShot[i].type)
			{
			case WZRD_FLAME: wzrdFlameShot(&plShot[i]); break;
			case WZRD_WATER: wzrdWaterShot(&plShot[i]); break;
			case WZRD_EARTH: wzrdEarthShot(&plShot[i]); break;
			}

		}

		break;
	}

}

void shot_render() {

	int i;

	for (i = 0; i < SHOT_MAX; i++)
	{
		if (plShot[i].exist == FALSE) continue;

		sprite_render(plShot[i].spr,
			plShot[i].pos.x,     plShot[i].pos.y,
			plShot[i].scale.x,   plShot[i].scale.y,
			plShot[i].texPos.x,  plShot[i].texPos.y,
			plShot[i].texSize.x, plShot[i].texSize.y,
			plShot[i].pivot.x,   plShot[i].pivot.y,
			plShot[i].angle,
			plShot[i].color.x,   plShot[i].color.y,
			plShot[i].color.z,   plShot[i].color.w);

		if (showHitCheck == TRUE) {
			primitive::rect(
				plShot[i].pos,
				plShot[i].texSize,
				plShot[i].pivot,
				plShot[i].angle,
				{ 0, 0, 1, 0.2f }
			);
		}
	}

}

void wzrdFlameShot(OBJ2D* obj) {

	switch (obj->act) {

	case 0:
		obj->spr     = sprFlameShot;
		obj->scale   = { SHOT_SCALE, SHOT_SCALE };
		obj->texPos  = { SHOT_TEX_POS_X, SHOT_TEX_POS_Y };
		obj->texSize = { SHOT_TEX_SIZE_X, SHOT_TEX_SIZE_Y};
		obj->pivot   = { SHOT_PIVOT_X, SHOT_PIVOT_Y};
		obj->color   = VEC4_WHITE;

		obj->pos    += {0, 30};
		obj->origin  = obj->pos;

		obj->hostilityFg = FALSE;     // –¡•û‚Å‚ ‚é

		obj->atk	 = WZRD_FLAME_ATK;

		obj->speed   = { SHOT_SPEED, 0 };

		obj->anime		= 0;
		obj->animeTimer = 0;
		obj->texPos		= { 0, 0 * obj->texSize.y};

		++obj->act;
		/*fallthrough*/

	case 1:
		////// ’Êí //////
		obj->pos += obj->speed;

		if (obj->pos.x > obj->origin.x + (SHOT_TEX_SIZE_X * 4)) obj->exist = FALSE;


		obj->anime = obj->animeTimer / 4 % 4;

		obj->texPos.x = obj->anime * obj->texSize.x;
		++obj->animeTimer;

		areaCheck(obj, 0);
		break;
	}
}
void wzrdWaterShot(OBJ2D* obj)
{
	switch (obj->act) {

	case 0:
		obj->spr = sprWaterShot;
		obj->scale = { SHOT_SCALE, SHOT_SCALE };
		obj->texPos = { SHOT_TEX_POS_X, SHOT_TEX_POS_Y };
		obj->texSize = { SHOT_TEX_SIZE_X, SHOT_TEX_SIZE_Y };
		obj->pivot = { SHOT_PIVOT_X, SHOT_PIVOT_Y };
		obj->color = VEC4_WHITE;

		obj->pos += {0, 30};
		obj->origin = obj->pos;

		obj->hostilityFg = FALSE;     // –¡•û‚Å‚ ‚é

		obj->atk = WZRD_WATER_ATK;

		obj->speed = { SHOT_SPEED, 0 };

		obj->anime = 0;
		obj->animeTimer = 0;
		obj->texPos = { 0, 0 * obj->texSize.y };

		++obj->act;
		/*fallthrough*/

	case 1:
		////// ’Êí //////
		obj->pos += obj->speed;

		if (obj->pos.x > obj->origin.x + (SHOT_TEX_SIZE_X * 4)) obj->exist = FALSE;


		obj->anime = obj->animeTimer / 4 % 4;

		obj->texPos.x = obj->anime * obj->texSize.x;
		++obj->animeTimer;

		areaCheck(obj, 0);
		break;
	}

}
void wzrdEarthShot(OBJ2D* obj)
{
	switch (obj->act) {

	case 0:
		obj->spr     = sprEarthShot;
		obj->scale   = { SHOT_SCALE, SHOT_SCALE };
		obj->texPos  = { SHOT_TEX_POS_X, SHOT_TEX_POS_Y };
		obj->texSize = { SHOT_TEX_SIZE_X, SHOT_TEX_SIZE_Y};
		obj->pivot   = { SHOT_PIVOT_X, SHOT_PIVOT_Y };
		obj->color   = VEC4_WHITE;

		obj->pos    += {0, 30};
		obj->origin  = obj->pos;

		obj->hostilityFg = FALSE;     // –¡•û‚Å‚ ‚é

		obj->atk = WZRD_EARTH_ATK;

		obj->speed = { SHOT_SPEED, 0 };

		obj->anime = 0;
		obj->animeTimer = 0;
		obj->texPos = { 0, 0 * obj->texSize.y };

		++obj->act;
		/*fallthrough*/

	case 1:
		////// ’Êí //////
		obj->pos += obj->speed;

		if (obj->pos.x > obj->origin.x + (SHOT_TEX_SIZE_X * 4)) obj->exist = FALSE;


		//obj->anime = obj->animeTimer / 4 % 4;

		//obj->texPos.x = obj->anime * obj->texSize.x;
		++obj->animeTimer;

		areaCheck(obj, 0);
		break;
	}

}
