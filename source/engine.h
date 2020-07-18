#pragma once
#include "includes.h"
#include "user_interface.h"
#include "chess.h"
#include "Game.h"

class engine
{
public:
	Game* currentGame = NULL;

	//TODO
	bool isMoveValid(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion);

	void makeTheMove(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion);

	void newGame(void);

	void undoMove(void);

	void movePiece(void);

	void saveGame(void);

	void loadGame(void);

	void runGame(engine engine);
};

