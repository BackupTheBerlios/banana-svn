SUBDIRS = gamegui mxml
INCLUDES = -I./gamegui/include
AM_CFLAGS = $(CFLAGS) -Wall -Werror -pedantic -ansi -std=c89 @SDL_CFLAGS@
noinst_HEADERS = main.h

bin_PROGRAMS = banana
banana_SOURCES = main.c gameloop.c scene.c camera.c init.c mouse.c tilebay.c texture.c gui.c text.c map.c title_dialog.c server_dialog.c client_dialog.c chat_dialog.c net_server.c net_client.c net_common.c player.c editmode.c anim.c

banana_LDADD = gamegui/libgamegui.a mxml/libmxml.a
banana_DEPENDENCIES = gamegui/libgamegui.a mxml/libmxml.a

