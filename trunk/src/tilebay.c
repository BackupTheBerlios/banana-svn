
#include "main.h"

texture_t tilebay;

texture_t *get_tilebay()
{
    return &tilebay;
}

void load_tilebay( char *filename )
{
    load_texture( &tilebay, filename, 1 );
}
