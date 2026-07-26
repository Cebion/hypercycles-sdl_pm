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
#include "Game.h"
#include <filesystem>

extern void main_hyper6(void);

Game game;

int main( int argc, char* argv[] )
{
	// The game loads all its data files (and reads/writes hyper.cfg/hyper.sav) via
	// bare relative fopen(), so if an "assets" folder sits next to the binary, run
	// from inside it. No-op for existing dev setups where the debugger/IDE already
	// starts the process inside "assets" itself (premake5.lua's debugdir).
	std::error_code ec;
	if( std::filesystem::is_directory( "assets", ec ) )
	{
		std::filesystem::current_path( "assets", ec );
	}

	if( !Game_Init( &game ) )
	{
		return 1;
	}

	main_hyper6();
	
	Game_Destroy( &game );

    return 0;
}
