#pragma once
#include <stdbool.h>

#define WORLD_SIZE_X		80
#define WORLD_SIZE_Y		25

#define WORLD_ANT_HOME_X    ( WORLD_SIZE_X / 2 )
#define WORLD_ANT_HOME_Y    ( WORLD_SIZE_Y / 2 )

#define FOOD_SIZE			10

// 0 = Black       8 = Gray
// 1 = Blue        9 = Light Blue
// 2 = Green       A = Light Green
// 3 = Aqua        B = Light Aqua
// 4 = Red         C = Light Red
// 5 = Purple      D = Light Purple
// 6 = Yellow      E = Light Yellow
// 7 = White       F = Bright White

#define COLOR_DEFAULT       0x07
#define COLOR_GRASS			0x22
#define COLOR_FOOD			0x2E
#define COLOR_COLONY		0x17
#define COLOR_ANT			0x20
#define COLOR_ANT_FOOD		0xA5
#define COLOR_PHER			0x27
#define COLOR_COUNT			0x57

enum world_tiles
{
	world_grass,
	world_food,
	world_colony
};

extern char world[ WORLD_SIZE_Y ][ WORLD_SIZE_X ];

void draw_world_tile( int x, int y );

int build_world();
int place_food( int in_x, int in_y, int w, int h );

bool is_valid_tile( int x, int y );