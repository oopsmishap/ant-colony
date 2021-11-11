#include <stdlib.h>
#include <time.h>

#include "console_funcs.h"
#include "world.h"
#include "ant.h"
#include "draw.h"

void simulation_loop()
{
	ant_t* head_ant = create_ant( WORLD_ANT_HOME_X, WORLD_ANT_HOME_Y );

	int tick = 0;
	int ants_died = 0;

	// TODO: Add checks to end the simulation

	while( true )
	{
		// currently spawning ants based on simulation ticks

		if( tick % 50 == 0 )
		{
			add_ant( &head_ant, WORLD_ANT_HOME_X, WORLD_ANT_HOME_Y );
		}

		ant_t* curr_ant = head_ant;

		int ants_alive = 0;

		while( curr_ant )
		{
			ant_think( curr_ant );

			if( is_ant_dead( curr_ant ) )
			{
				// delete from linked list
				kill_ant( head_ant, curr_ant );
				ants_died++;

				// removing item from list during loop is currently wonky
				// restarting the loop is a quick fix for now
				break;
			}

			curr_ant = curr_ant->next;
			ants_alive++;
		}

		// bottom info output
		draw_ants_count( ants_alive, ants_died, food_count );

		tick++;

		// slow simulation down as it's too fast
		Sleep( 30 );
	}
}

void simulation_init()
{
	setup_console();

	food_count = build_world();

	for( int x = 0; x < WORLD_SIZE_X; x++ )
	{
		for( int y = 0; y < WORLD_SIZE_Y; y++ )
		{
			draw_world_tile( x, y );
		}
	}

	simulation_loop();
}

int main()
{
	srand( time( NULL ) );

	simulation_init();

	set_colour( COLOR_DEFAULT );
	_cprintf( "\n" );

	return 0;
}