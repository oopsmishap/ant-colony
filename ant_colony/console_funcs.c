#include "console_funcs.h"

void set_colour( short colour )
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), colour );
}

void gotoxy( int x, int y )
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}

void setup_console()
{
	COORD screen_coord = { 0, 0 };
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO sbi;
	DWORD console_size;
	HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

	GetConsoleScreenBufferInfo( handle, &sbi );
	console_size = sbi.dwSize.X * sbi.dwSize.Y;
	FillConsoleOutputCharacter( handle, TEXT( ' ' ), console_size, screen_coord, &written );
	GetConsoleScreenBufferInfo( handle, &sbi );
	FillConsoleOutputAttribute( handle, sbi.wAttributes, console_size, screen_coord, &written );
	SetConsoleCursorPosition( handle, screen_coord );

	CONSOLE_CURSOR_INFO cci;

	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo( handle, &cci );
}