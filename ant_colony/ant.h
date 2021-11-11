#pragma once
#include <stdbool.h>

#include "world.h"

#define ANT_HP_START	250
#define ANT_FOOD_HP		100
#define ANT_MOVE_COST	1
#define PHER_AGE		2

enum ant_dir
{
	ant_n,
	ant_ne,
	ant_e,
	ant_se,
	ant_s,
	ant_sw,
	ant_w,
	ant_nw
};

typedef struct
{
	unsigned short uid;
	int x;
	int y;
	int prev_x;
	int prev_y;
	int home_x;
	int home_y;
	int hp;
	bool has_eaten;
	struct ant_t* next;
} ant_t;

typedef struct
{
	int uid;
	int dir_x;
	int dir_y;
	int age;
} pheromone_t;

extern pheromone_t	pheromones[ WORLD_SIZE_Y ][ WORLD_SIZE_X ];
extern int			food_count;

ant_t*	create_ant( int home_x, int home_y );
void	add_ant( ant_t** head, int home_x, int home_y );
void	delete_ant( ant_t** head, short uid );

void	ant_think( ant_t* ant );

bool	ant_search( ant_t* ant );
void	ant_search_move( ant_t* ant );
void	ant_eat( ant_t* ant );
void	ant_go_home( ant_t* ant );

void	move_ant( ant_t* ant, int dir_x, int dir_y );
void	kill_ant( ant_t* head, ant_t* ant );

bool	is_ant_dead( ant_t* ant );
bool	is_ant_going_home( ant_t* ant );
bool	is_ant_home( ant_t* ant );
bool	is_ant_on_food( ant_t* ant );