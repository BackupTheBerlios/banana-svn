
#include "main.h"

float camera_x, camera_y;

void set_camera( float xpos, float ypos )
{
    camera_x=xpos;
    camera_y=ypos;

    if ( camera_x<0 )
        camera_x=0;
    else if ( camera_x > ((get_player_layer()->width*32)-(SCREEN_WIDTH)) )
        camera_x=((get_player_layer()->width*32)-(SCREEN_WIDTH));

    if ( camera_y<0 )
        camera_y=0;
    else if ( camera_y > ((get_player_layer()->height*32)-(SCREEN_HEIGHT)) )
        camera_y=((get_player_layer()->height*32)-(SCREEN_HEIGHT));

   /* printf( "Camera: %f,%f\n", camera_x, camera_y ); */
}

void move_camera_on_player( int index )
{
    /* temporary */
    set_camera( get_player(index)->xpos-((SCREEN_WIDTH/2)-44), get_player(index)->ypos-((SCREEN_HEIGHT/2)-50) );
}

float get_camera_x()
{
    return camera_x;
}

float get_camera_y()
{
    return camera_y;
}
