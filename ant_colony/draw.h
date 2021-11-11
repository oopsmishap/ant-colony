#pragma once
#include "ant.h"

void draw_tile( int x, int y, short color, char ch );
void draw_ant( ant_t* ant );
void draw_ants_count( int alive, int dead, int food_left );