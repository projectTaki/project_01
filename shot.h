#pragma once


extern int   shot_state;
extern OBJ2D plShot[];

#define SHOT_MAX                (50)   // ショットの最大数

#define SHOT_SIZE_OFFSET        (0, -40)

// ショット用の定数を定義する
#define SHOT_TEX_POS_X          (0)    
#define SHOT_TEX_POS_Y          (0)	
#define SHOT_TEX_SIZE_X         (150)
#define SHOT_TEX_SIZE_Y         (150)
#define SHOT_PIVOT_X            (75)
#define SHOT_PIVOT_Y            (75)
#define SHOT_SCALE				(1.0f)
#define SHOT_SPEED              (20)   

/*#define ENEMY_SHOT_TEX_POS_X          (0)     
#define ENEMY_SHOT_TEX_POS_Y          (0)	
#define ENEMY_SHOT_TEX_SIZE_X         (150.0f)
#define ENEMY_SHOT_TEX_SIZE_Y         (150.0f)
#define ENEMY_SHOT_TEX_PIVOT_X        (75.0f) 
#define ENEMY_SHOT_TEX_PIVOT_Y        (75.0f) 
#define ENEMY_SHOT_SCALE			  (1.0f)
#define ENEMY_SHOT_SPEED              (15)    
#define ENEMY_SHOT_LIMIT              (32)    
#define ENEMY_SHOT_RADIUS             (65) */   


void shot_init();
void shot_deinit();
void shot_update();
void shot_render();

void wzrdFlameShot(OBJ2D*);
void wzrdWaterShot(OBJ2D*); 
void wzrdEarthShot(OBJ2D*);
