
#include "main.h"

/* Change so that the net code is checked/sent each loop, rather than a thread.. 
  leave the incoming server connection stuff in thread only. */


static char client_ip[80];
static int client_port;
static int server_port;

static char chat_buffer[500];
static int buffer_changed=FALSE;


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

void client_thread()
{
	IPaddress ip;		/* Server address */
	TCPsocket sd;		/* Socket descriptor */
	int quit, len;
    char buffer[512];
 
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, client_ip, client_port))
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
    //printf( "Server IP: %i:%i\n", ip.host, ip.port );

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Send messages */
	quit = 0;
	while (!quit)
	{
        SDL_Delay(100);
        if ( buffer_changed )        
        {
            //printf( "Sending buffer...\n" );
            len = strlen(chat_buffer) + 1;
            buffer[0]=MESG_CHAT;
            memcpy(buffer+1,chat_buffer,len);
            //printf( "Buffer: %s\n", buffer );

    		if (SDLNet_TCP_Send(sd, (void *)buffer, 512) < len)
    		{
    			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    			exit(EXIT_FAILURE);
    		}
            buffer_changed=FALSE;
     	}
        else if ( tile_changed )
        {
            len = 3;
            buffer[0]=MESG_TILE;
            buffer[1]=tile_x;
            buffer[2]=tile_y;

    		if (SDLNet_TCP_Send(sd, (void *)buffer, 512) < len)
    		{
    			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    			exit(EXIT_FAILURE);
    		}

            tile_changed=FALSE;
        }
    }
 
	SDLNet_TCP_Close(sd);
}

void client_listen( char *ip, int port )
{
    SDL_Thread *thread;

    printf( "Connecting to %s on port %i\n", ip, port );

    sprintf(client_ip, "%s", ip );
    client_port=port;

    thread = SDL_CreateThread(client_thread, NULL);
    
    if ( thread == NULL ) 
    {
        fprintf(stderr, "Unable to create client_listen thread: %s\n", SDL_GetError());
        return;
    }

    //SDL_WaitThread(thread, NULL);
}

void server_thread()
{
	TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
	IPaddress ip, *remoteIP;
	int quit, quit2;
	char buffer[512];
 
	if (SDLNet_ResolveHost(&ip, NULL, server_port))
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    //ip.port=5555;
 
   // printf( "Server IP: %i:%i\n", ip.host, ip.port );
    

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Wait a connection, send data and term */
	quit = 0;
	while (!quit)
	{
        SDL_Delay(100);
		/* This check the sd if there is a pending connection.
		* If there is one, accept that, and open a new socket for communicating */
		if ((csd = SDLNet_TCP_Accept(sd)))
		{
            //printf( "Connection pending!\n" );
			/* Now we can communicate with the client using csd socket
			* sd will remain opened waiting other connections */
 
			/* Get the remote address */
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				/* Print the address, converting in the host format */
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
			
			quit2 = 0;
			while (!quit2)
			{
                SDL_Delay(100);
				if (SDLNet_TCP_Recv(csd, buffer, 512))
				{
                    switch( buffer[0] )
                    {
                        case MESG_CHAT:
                            //printf( "Chat message\n" );
                            memcpy(chat_buffer, buffer+1, 500);
                            //printf( "Chat buffer is now: %s\n", chat_buffer );
                            break;
                        case MESG_TILE:
                            //printf( "Tile message\n" );
                            set_tile(get_player_layer(), buffer[1], buffer[2], 1);
                            //printf( "%i,%i\n", buffer[1], buffer[2] );
                            break;
                    }
					//printf("Client say: %s\n", buffer);
                    //sprintf( chat_buffer, "%s", buffer );
 
    			}
			}
 
			/* Close the client socket */
			SDLNet_TCP_Close(csd);
		}
	}
 
	SDLNet_TCP_Close(sd);
}

void server_listen( int port )
{
    SDL_Thread *thread;

    printf( "Listening on port %i\n", port );

    server_port = port;

    thread = SDL_CreateThread(server_thread, NULL);
    
    if ( thread == NULL ) 
    {
        fprintf(stderr, "Unable to create server_listen thread: %s\n", SDL_GetError());
        return;
    }

    //SDL_WaitThread(thread, NULL);
}

