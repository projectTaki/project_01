#pragma once
using namespace GameLib;

#define HP_HP         (10)
#define HP_POS_X      (38)
#define HP_POS_Y      (795)
#define HP_SCALE      (1.0f)
#define HP_TEX_SIZE_X (600)
#define HP_TEX_SIZE_Y (200)

#define ELM_POS_X      (30)
#define ELM_POS_Y      (895)
#define ELM_SCALE      (1.0f)
#define ELM_TEX_SIZE_X (600)
#define ELM_TEX_SIZE_Y (200)

#define FEVER_TIME     (15 *60)

extern int   ui_state;
extern int   cost;

extern int   time_limit;

extern BOOL  feverAlready;

extern OBJ2D   hp;
extern OBJ2D   element;
extern OBJ2D   feverGauge;
extern OBJ2D   feverText;
extern OBJ2D   feverButton;
extern OBJ2D   bg;

extern Sprite* sprFeverGauge;
extern Sprite* sprFeverFrame;
extern Sprite* sprFeverText;

void ui_init();
void ui_deinit();
void ui_update();
void ui_render();