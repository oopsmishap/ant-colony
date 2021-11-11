#include "draw.h"
#include "world.h"
#include "console_funcs.h"

void draw_tile( int x, int y, short color, char ch )
{
	set_colour( color );
	gotoxy( x, y );
	_cprintf( "%c", ch );
}

void draw_ant( ant_t* ant )
{
	// redraw tile ant just left to world tile

	draw_world_tile( ant->prev_x, ant->prev_y );

	// ant has eaten and returning home

	if( is_ant_going_home( ant ) )
	{
		set_colour( COLOR_ANT_FOOD );
		gotoxy( ant->x, ant->y );
		_cprintf( "A" );
	}
	else
	{
		set_colour( COLOR_ANT );
		gotoxy( ant->x, ant->y );
		_cprintf( "A" );
	}
}

void draw_ants_count( int alive, int dead, int food_left )
{
	set_colour( COLOR_COUNT );
	gotoxy( 0, WORLD_SIZE_Y );
	_cprintf( "Ants Alive: %05d\t\tAnts Died: %05d\t\tFood Left: %05d", alive, dead, food_left );
}