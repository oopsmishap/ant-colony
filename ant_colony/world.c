#include "world.h"
#include "ant.h"
#include "draw.h"

#include <stdlib.h>

char world[ WORLD_SIZE_Y ][ WORLD_SIZE_X ] = { 0 };

void draw_world_tile( int x, int y )
{
	enum tile_type type = world[ y ][ x ];
	pheromone_t pher = pheromones[ y ][ x ];

	// a valid pheromone has precedence 

	if( pher.age > 0 )
	{
		draw_tile( x, y, COLOR_PHER, '.' );
		return;
	}

	switch( type )
	{
	case world_grass:
		draw_tile( x, y, COLOR_GRASS, ' ' );
		break;
	case world_food:
		draw_tile( x, y, COLOR_FOOD, 'o' );
		break;
	case world_colony:
		draw_tile( x, y, COLOR_COLONY, 'C' );
		break;
	default:
		break;
	}
}

int build_world()
{
	world[ WORLD_ANT_HOME_Y ][ WORLD_ANT_HOME_X ] = world_colony;

	int food_count = 0;

	food_count += place_food( 0, 0, FOOD_SIZE, FOOD_SIZE );
	food_count += place_food( WORLD_SIZE_X - FOOD_SIZE, 0, FOOD_SIZE, FOOD_SIZE );
	food_count += place_food( 0, WORLD_SIZE_Y - FOOD_SIZE, FOOD_SIZE, FOOD_SIZE );
	food_count += place_food( WORLD_SIZE_X - FOOD_SIZE, WORLD_SIZE_Y - FOOD_SIZE, FOOD_SIZE, FOOD_SIZE );

	return food_count;
}

int place_food( int in_x, int in_y, int w, int h )
{
	int food_count = 0;

	for( int y = in_y; y < ( in_y + h ); y++ )
	{
		for( int x = in_x; x < ( in_x + w ); x++ )
		{
			bool food = rand() % 2;
			if( food )
			{
				world[ y ][ x ] = world_food;
				food_count++;
			}
		}
	}

	return food_count;
}

bool is_valid_tile( int x, int y )
{
	if( 0 <= x && x <= ( WORLD_SIZE_X - 1 ) &&
		0 <= y && y <= ( WORLD_SIZE_Y - 1 ) )
	{
		return true;
	}

	return false;
}
