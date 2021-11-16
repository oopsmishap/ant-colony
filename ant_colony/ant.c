#include "ant.h"
#include "draw.h"

#include <stdlib.h>
#include <memory.h>

pheromone_t pheromones[ WORLD_SIZE_Y ][ WORLD_SIZE_X ] = { 0 };

int food_count = 0;

/*------------------------------------------------*/

ant_t* create_ant( int home_x, int home_y )
{
	ant_t* ant = NULL;

	ant = ( ant_t* )malloc( sizeof( ant_t ) );

	memset( ant, NULL, sizeof( ant_t ) );

	ant->uid = rand() % USHRT_MAX;
	ant->x = home_x;
	ant->y = home_y;
	ant->prev_x = home_x;
	ant->prev_y = home_y;
	ant->home_x = home_x;
	ant->home_y = home_y;
	ant->hp = ANT_HP_START;

	return ant;
}

void add_ant( ant_t** ant, int home_x, int home_y )
{
	ant_t* new_ant = create_ant( home_x, home_y );

	new_ant->next = *ant;
	*ant = new_ant;
}

void delete_ant( ant_t** head, short uid )
{
	ant_t* temp = *head;
	ant_t* prev = NULL;

	if( temp != NULL && temp->uid == uid )
	{
		*head = temp->next;
		free( temp );
		return;
	}

	while( temp != NULL && temp->uid != uid )
	{
		prev = temp;
		temp = temp->next;
	}

	if( temp == NULL )
		return;

	prev->next = temp->next;
	free( temp );
}

/*------------------------------------------------*/

void ant_think( ant_t* ant )
{
	if( is_ant_dead( ant ) )
		return;

	if( is_ant_going_home( ant ) )
	{
		if( is_ant_home( ant ) )
		{
			ant->has_eaten = false;	
		}
		else
		{
			ant_go_home( ant );
		}
	}
	else
	{
		if( ant_search( ant ) )
		{
			if( is_ant_on_food( ant ) )
			{
				ant_eat( ant );
			}
		}
		else
		{
			ant_search_move( ant );
		}
	}

	draw_ant( ant );
}

/*------------------------------------------------*/

bool ant_search( ant_t* ant )
{
	// search 3x3 square around ant for food

	for( int y = ant->y - 1; y < ( ant->y + 3 ); y++ )
	{
		for( int x = ant->x - 1; x < ( ant->x + 3 ); x++ )
		{
			if( is_valid_tile( x, y ) == false )
				continue;

			if( world[ y ][ x ] == world_food )
			{
				int dir_x = x - ant->x;
				int dir_y = y - ant->y;
				move_ant( ant, dir_x, dir_y );

				return true;
			}
		}
	}

	// if no food check if ant is on pheromone tile

	if( pheromones[ ant->y ][ ant->x ].uid != 0 )
	{
		int x = ant->x;
		int y = ant->y;
		int dir_x = pheromones[ y ][ x ].dir_x;
		int dir_y = pheromones[ y ][ x ].dir_y;

		move_ant( ant, dir_x, dir_y );


		// TODO: maybe change this so age decrements over time/simulation ticks 
		//		 instead of how many times it's been walked on
		pheromones[ y ][ x ].age -= 1;

		if( pheromones[ y ][ x ].age == 0 )
		{
			pheromones[ y ][ x ].uid = 0;
			pheromones[ y ][ x ].dir_x = 0;
			pheromones[ y ][ x ].dir_y = 0;
		}

		return true;
	}

	// if not on phermone tile search 3x3 square around ant for one

	for( int y = ant->y - 1; y < ( ant->y + 3 ); y++ )
	{
		for( int x = ant->x - 1; x < ( ant->x + 3 ); x++ )
		{
			if( is_valid_tile( x, y ) == false )
				continue;

			if( pheromones[ y ][ x ].uid != 0 )
			{
				int dir_x = x - ant->x;
				int dir_y = y - ant->y;
				move_ant( ant, dir_x, dir_y );

				return true;
			}
		}
	}

	return false;
}

void ant_search_move( ant_t* ant )
{
	enum ant_dir dir = rand() % 8;

	switch( dir )
	{
	case ant_n:
		move_ant( ant, 0, -1 );
		break;
	case ant_ne:
		move_ant( ant, +1, -1 );
		break;
	case ant_e:
		move_ant( ant, +1, 0 );
		break;
	case ant_se:
		move_ant( ant, +1, +1 );
		break;
	case ant_s:
		move_ant( ant, 0, +1 );
		break;
	case ant_sw:
		move_ant( ant, -1, +1 );
		break;
	case ant_w:
		move_ant( ant, -1, 0 );
		break;
	case ant_nw:
		move_ant( ant, -1, -1 );
		break;

	default:
		break;
	}
}

/*------------------------------------------------*/

void ant_eat( ant_t* ant )
{
	if( world[ ant->y ][ ant->x ] == world_food )
	{
		world[ ant->y ][ ant->x ] = world_grass;
		ant->hp = ant->hp + ANT_FOOD_HP;
		ant->has_eaten = true;

		food_count--; // dirty global for info bar
	}
}

void ant_go_home( ant_t* ant )
{
	// work out home direction by finding the delta

	int home_dir_x = ant->x - ant->home_x;
	int home_dir_y = ant->y - ant->home_y;

	int dir_x = 0;
	int dir_y = 0;

	// save current pos for phermone

	int phermone_x = ant->x;
	int phermone_y = ant->y;

	// if home_dir is positive then ant needs to move closer to origin
	// if home_dir is negitive then ant needs to move away from origin

	if( home_dir_x > 0 )
	{
		dir_x = -1;
	}
	else if( home_dir_x < 0 )
	{
		dir_x = 1;
	}

	if( home_dir_y > 0 )
	{
		dir_y = -1;
	}
	else if( home_dir_y < 0 )
	{
		dir_y = 1;
	}

	// move ant based on home-dir delta

	move_ant( ant, dir_x, dir_y );

	// as ant has found food and returning home place phermone

	pheromones[ phermone_y ][ phermone_x ].uid = ant->uid;
	pheromones[ phermone_y ][ phermone_x ].dir_x = -dir_x; // flip sign
	pheromones[ phermone_y ][ phermone_x ].dir_y = -dir_y; // flip sign
	pheromones[ phermone_y ][ phermone_x ].age = PHER_AGE;
}

void move_ant( ant_t* ant, int dir_x, int dir_y )
{
	if( is_valid_tile( ant->x + dir_x, ant->y + dir_y ) )
	{
		// save pos before moving to allow world tile redraw
		
		ant->prev_x = ant->x;
		ant->prev_y = ant->y;
		
		ant->x = ant->x + dir_x;
		ant->y = ant->y + dir_y;
		
		// decrement ant's health on move
		
		ant->hp -= ANT_MOVE_COST;
	}
}

void kill_ant( ant_t* head, ant_t* ant )
{
	// draw over ant with world tile
	draw_world_tile( ant->x, ant->y );
	// remove from linekd list
	delete_ant( &head, ant->uid );
}

/*------------------------------------------------*/

bool is_ant_dead( ant_t* ant )
{
	return ( 0 > ant->hp ) ? true : false;
}

bool is_ant_going_home( ant_t* ant )
{
	return ant->has_eaten;
}

bool is_ant_home( ant_t * ant )
{
	if( ant->x == ant->home_x &&
		ant->y == ant->home_y )
	{
		return true;
	}

	return false;
}

bool is_ant_on_food( ant_t* ant )
{
	return ( world[ ant->y ][ ant->x ] == world_food ) ? true : false;
}
