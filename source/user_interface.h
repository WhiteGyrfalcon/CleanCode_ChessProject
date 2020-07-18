#pragma once
#include "chess.h"
#include "Game.h"

void createNextMessage( string message );
void appendToNextMessage( string message );
void clearScreen( void );
void printLogo( void );
void printLogo( void );
void printMenu( void );
void printMessage( void );
void printLine( int line, int colorToStart, int colorAfterStart, Game& game );
void printHistory( Game& game );
void printCaptured(Game& game);
void printBoard(Game& game);