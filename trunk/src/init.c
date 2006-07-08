
#include "main.h"

SDL_Surface *screen;

void init_gl( GLvoid );
void resize_window( int width, int height );

int videoFlags;

void init_net()
{
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
}

void init_video()
{
    const SDL_VideoInfo *videoInfo;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError() );
	    quit( 1 );
	}

    videoInfo = SDL_GetVideoInfo();
    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n", SDL_GetError() );
	    quit( 1 );
	}

    videoFlags  = SDL_OPENGL;          
    videoFlags |= SDL_GL_DOUBLEBUFFER; 
   /* videoFlags |= SDL_HWPALETTE;       */
    /*videoFlags |= SDL_RESIZABLE;       */

 /*   if ( videoInfo->hw_available )
    	videoFlags |= SDL_HWSURFACE;
    else
    	videoFlags |= SDL_SWSURFACE;

    if ( videoInfo->blit_hw )
    	videoFlags |= SDL_HWACCEL;
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );*/

    SDL_EnableUNICODE(TRUE);

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,	videoFlags );
    if ( !screen )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    quit( 1 );
	}

    init_gl();
    resize_window( SCREEN_WIDTH, SCREEN_HEIGHT );  
}

void init_gl( GLvoid )
{
    glShadeModel( GL_SMOOTH );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClearDepth( 1.0f );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void resize_window( int width, int height )
{
    GLfloat ratio;

    if ( height == 0 )
	   height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho(0,640,480,0,0,100);	
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

int get_videoflags()
{
    return videoFlags;
}
