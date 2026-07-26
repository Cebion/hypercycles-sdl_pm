/*
* hypercycles-sdl (https://github.com/Herschel/hypercycles-sdl)
* Copyright (c) 2015 The hypercycles-sdl authors
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "prelude.h"
#include <ctype.h>
#include <stdio.h>
#include "SDL.h"

int sdl_key;
extern int raw_key;

// The game's own key handling (New_Key_Int() in hyper6.cpp) expects real IBM PC
// keyboard scancodes (Set 1), not SDL keycodes - e.g. it reads scancode 32 as the
// 'D' key, but SDL's keycode for the Space bar is also 32 (ASCII ' '). Anything
// not covered here falls back to the raw SDL keycode, matching prior behavior.
static int SDLKeyToDosScancode(SDL_Keycode sym)
{
	switch (sym)
	{
		case SDLK_1: return 2;
		case SDLK_2: return 3;
		case SDLK_3: return 4;
		case SDLK_4: return 5;
		case SDLK_5: return 6;
		case SDLK_6: return 7;
		case SDLK_7: return 8;
		case SDLK_8: return 9;
		case SDLK_9: return 10;
		case SDLK_0: return 11;
		case SDLK_BACKSPACE: return 14;
		case SDLK_q: return 16;
		case SDLK_w: return 17;
		case SDLK_e: return 18;
		case SDLK_r: return 19;
		case SDLK_t: return 20;
		case SDLK_y: return 21;
		case SDLK_u: return 22;
		case SDLK_i: return 23;
		case SDLK_o: return 24;
		case SDLK_p: return 25;
		case SDLK_a: return 30;
		case SDLK_s: return 31;
		case SDLK_d: return 32;
		case SDLK_f: return 33;
		case SDLK_g: return 34;
		case SDLK_h: return 35;
		case SDLK_j: return 36;
		case SDLK_k: return 37;
		case SDLK_l: return 38;
		case SDLK_z: return 44;
		case SDLK_x: return 45;
		case SDLK_c: return 46;
		case SDLK_v: return 47;
		case SDLK_b: return 48;
		case SDLK_n: return 49;
		case SDLK_m: return 50;
		case SDLK_SPACE: return 57;
		case SDLK_LCTRL:
		case SDLK_RCTRL: return 29;
		case SDLK_LALT:
		case SDLK_RALT: return 56;
		default: return 0;
	}
}

// Break (key-release) scancodes New_Key_Int() actually reacts to (h3d_mdef.h's
// BREAK_* for the arrow keys, plus 157 for Ctrl release - see hyper6.cpp's
// "case 157: // Ctrl Key unpressed"). Anything else has no release behavior
// in the game, so is deliberately left unhandled rather than passed through.
static int SDLKeyToDosBreakScancode(SDL_Keycode sym)
{
	switch (sym)
	{
		case SDLK_UP: return 200;    // BREAK_UP
		case SDLK_DOWN: return 208;  // BREAK_DOWN
		case SDLK_LEFT: return 203;  // BREAK_LEFT
		case SDLK_RIGHT: return 205; // BREAK_RIGHT
		case SDLK_LCTRL:
		case SDLK_RCTRL: return 157;
		default: return 0;
	}
}

// TODO(mike): Temporary to make loading work on case-sensitive platforms.
// Temporary until ADT file loading code is cleaned up.
char* strupper(char* s)
{
	while (*s != 0)
	{
		*s = toupper(*s);
		s++;
	}
	return s;
}

char* strlower( char* s )
{
	while( *s != 0 )
	{
		*s = tolower( *s );
		s++;
	}
	return s;
}

char* int_to_str(int value, char* str, int base)
{
	sprintf(str, "%d", value);
	return str;
}

long filelen( FILE* fd )
{
	long len = 0;
	if( fd != NULL && fseek( fd, 0, SEEK_END ) == 0 )
	{
		len = ftell( fd );
		fseek( fd, 0, SEEK_SET );
	}
	return len;
}

int getch( void )
{
	return 0;
}

int inp( int a )
{
	return 0;
}

void outp(int a, int b)
{
}

int kbhit( void )
{
	return 0;
}

// TODO: Hoist this all out into the main loop.
// Will have to tear apart the mcp1 function...
void delay(int ms)
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 320;
	r.h = 200;


	unsigned int* pixels;
	int pitch;
	SDL_LockTexture(game.screen, &r, (void**)&pixels, &pitch);
	for (int i = 0; i < 320 * 200; i++)
	{
		unsigned char pal = vga_ram_c[i];
		unsigned int r = red[ pal ] << 2;
		unsigned int g = green[pal] << 2;
		unsigned int b = blue[pal] << 2;
		pixels[i] = (r << 24) | (g << 16) | (b << 8) | 0xff;
	}
	SDL_UnlockTexture(game.screen);
	
	SDL_RenderCopy(game.renderer, game.screen, NULL, NULL);
	SDL_RenderPresent(game.renderer);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		/* handle your event here */
		switch (event.type)
		{

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				sdl_key = 1;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				sdl_key = 72;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				sdl_key = 80;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				sdl_key = 75;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				sdl_key = 77;
			}
			else if (event.key.keysym.sym == SDLK_RETURN)
			{
				sdl_key = 28;
			}
			else
			{
				int scancode = SDLKeyToDosScancode(event.key.keysym.sym);
				sdl_key = scancode ? scancode : (int)event.key.keysym.sym;
			}
			New_Key_Int();
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				// raw_key has no release/break handling of its own (New_Key_Int()
				// only ever sets it, never clears it), so once Escape opens or
				// closes a menu, raw_key stays stuck at 1 forever. The main loop's
				// own esc_chk cooldown only delays the fallout: a few frames after
				// closing a menu, the stale raw_key==1 reopens it again with no
				// further input at all. Clear it directly on release.
				raw_key = 0;
			}
			else
			{
				int breakScancode = SDLKeyToDosBreakScancode(event.key.keysym.sym);
				if (breakScancode)
				{
					sdl_key = breakScancode;
					New_Key_Int();
				}
			}
			break;

		case SDL_QUIT:
			exit(0);
			break;
		}
	}


	SDL_Delay( ms );
}

const char* getcmd()
{
	printf("getcmd\n");
	return "";
}

int cprintf( const char * format, ... )
{
	return 0;
}

