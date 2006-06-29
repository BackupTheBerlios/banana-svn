
#include "main.h"

/* Change so that the net code is checked/sent each loop, rather than a thread.. 
  leave the incoming server connection stuff in thread only. */

/*typedef struct player
{
    float xpos, ypos;
    char name[80];

    IPaddress *ip;
    TCPsocket socket;
}player_t;*/

static int num_players=0;
static player_t players[16];

static char client_host[80];
static int client_port;

static char chat_buffer[500];
static int buffer_changed=FALSE;

static int client_active=FALSE;
static IPaddress client_ip;		/* Server address */
static TCPsocket client_sd;		/* Socket descriptor */

#define MESG_CHAT   'A'
#define MESG_TILE   'B'

/* Temporary!!*/
static int tile_x=0;
static int tile_y=0;
static int tile_changed=FALSE;

void net_change_tile( int x, int y )
{
    tile_changed=TRUE;
    tile_x=x;
    tile_y=y;
   // printf( "Tile changed: %i, %i\n", x, y );
}

/* -----^ Temporary! */

void update_buffer( char *text )
{
    sprintf( chat_buffer, "%s", text );
    buffer_changed=TRUE;

    //printf( "Buffer changed! %s\n", chat_buffer );    
}

char *get_chat_buffer()
{
    return chat_buffer;
}

void process_client()
{
    int len;
    char net_buffer[512];

    if (!client_active)
        return;
 
    if ( buffer_changed )        
    {
        //printf( "Sending buffer...\n" );
        len = strlen(chat_buffer) + 1;
        net_buffer[0]=MESG_CHAT;
        memcpy(net_buffer+1,chat_buffer,len);
        //printf( "Buffer: %s\n", buffer );

  		if (SDLNet_TCP_Send(client_sd, (void *)net_buffer, 512) < len)
  		{
  			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
   			exit(EXIT_FAILURE);
   		}
        buffer_changed=FALSE;
   	}
    else if ( tile_changed )
    {
        len = 3;
        net_buffer[0]=MESG_TILE;
        net_buffer[1]=tile_x;
        net_buffer[2]=tile_y;

        if (SDLNet_TCP_Send(client_sd, (void *)net_buffer, 512) < len)
    	{
    		fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    		exit(EXIT_FAILURE);
    	}

        tile_changed=FALSE;
    }
}

void client_listen( char *ip, int port )
{
   // SDL_Thread *thread;

    printf( "Connecting to %s on port %i\n", ip, port );

    sprintf(client_host, "%s", ip );
    client_port=port;

	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&client_ip, client_host, client_port))
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
    //printf( "Server IP: %i:%i\n", ip.host, ip.port );

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(client_sd = SDLNet_TCP_Open(&client_ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    client_active=TRUE;

  //  thread = SDL_CreateThread(client_thread, NULL);
    
  //  if ( thread == NULL ) 
  //  {
  //      fprintf(stderr, "Unable to create client_listen thread: %s\n", SDL_GetError());
  //      return;
  //  }

    //SDL_WaitThread(thread, NULL);
}


