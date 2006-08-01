
/*
    Banana - A multiplayer platform game engine.
    Copyright(C) 2006, Kris McAulay  

    text.c - Load fonts. 
*/

#include "main.h"

void draw_anim( anim_t *a, int frame, int xpos, int ypos, int flip )
{
    float u1, u2, v1, v2;

    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, a->tex->gl_index );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTranslatef( xpos, ypos, 0.0f );

    if ( !flip )
    {
        u1=a->frames[frame].upos; 
        u2=a->frames[frame].upos+a->uwidth;
    }
    else
    {
        u1=a->frames[frame].upos+a->uwidth;
        u2=a->frames[frame].upos; 
    }
   
    v1=a->frames[frame].vpos; v2=a->frames[frame].vpos+a->vheight;

    glBegin( GL_QUADS ); 
        glTexCoord2f( u1, v2 ); /* Top Left */
        glVertex3f( 0.0f,  0.0f, 0.0f );
        glTexCoord2f( u2, v2 ); /* Top Right */
        glVertex3f( a->width*2, 0.0f, 0.0f );
        glTexCoord2f( u2, v1 ); /* Bottom Right */
        glVertex3f( a->width*2, a->height*2,  0.0f );
        glTexCoord2f( u1, v1 ); /* Bottom Left */
        glVertex3f( 0.0f,  a->height*2,  0.0f );
    glEnd( ); 

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
}


#ifdef FART
static void load_opaque(mxml_node_t *top, char *name, char *dest)
{
    mxml_node_t *node = mxmlFindElement(top, top, name, NULL, NULL, MXML_DESCEND);
    if (node)
    {
        node = mxmlWalkNext(node, node, MXML_DESCEND);
        if (node && node->type == MXML_OPAQUE)
            strcpy(dest, node->value.opaque);
    }
}

static void load_anim( char *filename )
{
    FILE *fp;
    //char temp[256];
    mxml_node_t *tree, *save;

    fp = fopen(filename, "r");
    if (fp)
    {
        tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
        slots |= (1 << slot);
    }
    else
    {
        /*printf( "Error opening theme file.\n" );*/
        sprintf( desc, "Empty." );
        slots &= ~(1 << slot);
        return;
    }

    fclose(fp);

    save = tree;

    while ((save = mxmlFindElement(save, tree, "save", NULL, NULL, MXML_DESCEND)))
    {
        mxml_node_t *node;
        time_t time;
        struct tm *tm;
        board_t *board;

        load_opaque(save, "time", temp);

        time = atoi(temp);
        tm = localtime(&time);
        sprintf(time_save[slot], "%02i/%02i at %02i:%02i.", tm->tm_mday, tm->tm_mon,
                tm->tm_hour, tm->tm_min);

        load_opaque(save, "white", temp);
        if (!strcmp(temp, "ui"))
            config_save[slot].player[WHITE] = PLAYER_UI;
        else
            config_save[slot].player[WHITE] = PLAYER_ENGINE;

        load_opaque(save, "black", temp);
        if (!strcmp(temp, "ui"))
            config_save[slot].player[BLACK] = PLAYER_UI;
        else
            config_save[slot].player[BLACK] = PLAYER_ENGINE;

        load_opaque(save, "level", temp);
        config_save[slot].cpu_level = atoi(temp);

        load_opaque(save, "fen", temp);
        board = fen_decode(temp);
        if (board)
        {
            saved_board[slot] = *board;
            free(board);
        }
    }
}
#endif
