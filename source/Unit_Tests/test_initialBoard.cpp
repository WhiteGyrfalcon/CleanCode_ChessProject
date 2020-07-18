#include "../Game.h"
#include "../doctest.h"

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

TEST_CASE("Test if rook is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(0, 0);

	CHECK(test_initialBoard[0][0] == testFigure);
}

TEST_CASE("Test if king is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(0, 4);

	CHECK(test_initialBoard[0][4] == testFigure);
}

TEST_CASE("Test if nothing is placed on a supposed empty space") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(4, 4);

	CHECK(test_initialBoard[4][4] == testFigure);
}

TEST_CASE("Test if a pawn is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(1, 0);

	CHECK(test_initialBoard[1][0] == testFigure);
}

TEST_CASE("Test if a queen is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(7, 3);

	CHECK(test_initialBoard[7][3] == testFigure);
}

TEST_CASE("Test if a knight is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(7, 1);

	CHECK(test_initialBoard[7][1] == testFigure);
}

TEST_CASE("Test if a bishop is placed allright") {
	Game* testGame = new Game();

	char testFigure = testGame->getPieceAtPosition(7, 6);

	CHECK(test_initialBoard[7][6] == testFigure);
}
