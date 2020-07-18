#include "../engine.h"
#include "../doctest.h"

TEST_CASE("Test if undo works alright when a move has been made") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	bool test = testEngine->currentGame->undoIsPossible();

	CHECK(test);

}

TEST_CASE("Test if undo works alright no move is made so no undo") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->currentGame->undoIsPossible();

	CHECK(test == false);

}