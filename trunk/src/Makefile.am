SUBDIRS = gamegui mxml
INCLUDES = -I./gamegui/include
AM_CFLAGS = $(CFLAGS) -Wall -Werror -pedantic -ansi -std=c89 @SDL_CFLAGS@
noinst_HEADERS = main.h

bin_PROGRAMS = banana
banana_SOURCES = main.c gameloop.c scene.c camera.c init.c mouse.c tilebay.c texture.c gui.c \
text.c map.c net_server.c net_client.c net_common.c player.c editmode.c anim.c \
dialogs/title.c dialogs/mesg.c dialogs/client.c dialogs/chat.c dialogs/server.c dialogs/tile_select.c \
dialogs/current_tiles.c

banana_LDADD = gamegui/libgamegui.a mxml/libmxml.a
banana_DEPENDENCIES = gamegui/libgamegui.a mxml/libmxml.a

