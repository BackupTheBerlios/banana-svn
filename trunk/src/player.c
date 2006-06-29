
#include "main.h"

int num_players=0;
player_t players[16];

player_t *get_player( int index )
{
    return &players[index];
}

void set_num_players( int count )
{
    num_players=count;
}

int get_num_players()
{
    return num_players;
}
