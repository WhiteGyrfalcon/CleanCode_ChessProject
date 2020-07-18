#include "../engine.h"
#include "../doctest.h"

TEST_CASE("Test if empty square can be moved") {
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

	Position current = { 3,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	char testFigure1 = testGame->getPieceAtPosition(3, 0);
	char testFigure2 = testGame->getPieceAtPosition(4, 0);

	CHECK(test_initialBoard[3][0] == testFigure1);
	CHECK(test_initialBoard[4][0] == testFigure2);
}

//Pawn tests

TEST_CASE("Test if pawn doesnt move when asked to move to the same square") {
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

	Position current = { 6,0 };
	Position future = { 6,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);
	char testFigure = testGame->getPieceAtPosition(6, 0);

	CHECK(test_initialBoard[6][0] == testFigure);
}

TEST_CASE("Test if pawn can move backwards") {
	Game* testGame = new Game();
	engine* testEngine = new engine();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { EMPTY_SQUARE,  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->currentGame = new Game();

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);
	testEngine->makeTheMove({ 1,0 }, { 3, 0 }, &enPassant, &castling, &promotion);
	bool test = testEngine->isMoveValid({ 4,0 }, { 5, 0 }, &enPassant, &castling, &promotion);

	CHECK(test == false);
}

TEST_CASE("Test if pawn moves allright 2 squares") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);
	char testFigure = testGame->getPieceAtPosition(6, 0);

	CHECK(test_initialBoard[6][0] == testFigure);
}

TEST_CASE("Test if pawn moves allright 2 squares") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);
	char testFigure = testGame->getPieceAtPosition(4, 0);

	CHECK(test_initialBoard[4][0] == testFigure);
}

TEST_CASE("Test if pawn moves allright 1 square") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 5,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);
	char testFigure = testGame->getPieceAtPosition(5, 0);

	CHECK(test_initialBoard[5][0] == testFigure);
}

TEST_CASE("Test if pawn moves allright 1 square") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 5,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);
	char testFigure = testGame->getPieceAtPosition(6, 0);

	CHECK(test_initialBoard[6][0] == testFigure);
}

TEST_CASE("Test if pawn doesnt move when blocked") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { EMPTY_SQUARE,  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,0 }, { 3, 0 }, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 4,0 }, { 3, 0 }, &enPassant, &castling, &promotion);//moving white pawn
	char testFigure = testGame->getPieceAtPosition(5, 0);

	CHECK(test_initialBoard[5][0] == testFigure);
}

TEST_CASE("Test if pawn takes enemy figures properly") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { EMPTY_SQUARE,  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,1 }, { 3, 1 }, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 4,0 }, { 3, 1 }, &enPassant, &castling, &promotion);//moving white pawn
	char testFigure1 = testGame->getPieceAtPosition(3, 1);
	char testFigure2 = testGame->getPieceAtPosition(4, 0);

	CHECK(test_initialBoard[3][1] == testFigure1);
	CHECK(test_initialBoard[4][0] == testFigure2);
}

TEST_CASE("Test EnPassant") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  EMPTY_SQUARE,  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,1 }, { 3, 1 }, &enPassant, &castling, &promotion);//moving black pawn
	testGame->movePiece({ 4,0 }, { 3, 0 }, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 3,1 }, { 4, 0 }, &enPassant, &castling, &promotion);//moving black pawn
	char testFigure = testGame->getPieceAtPosition(4, 0);

	CHECK(test_initialBoard[4][0] == testFigure);
}

TEST_CASE("Test if pawn can move when under check") {
	Game* testGame = new Game();
	engine* testEngine = new engine();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  EMPTY_SQUARE,  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  EMPTY_SQUARE,  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, 'B', EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p' },
	   { 'p',  'p',  'p',  'p',  EMPTY_SQUARE,  'p',  'p',  EMPTY_SQUARE },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,4 };
	Position future = { 4,4 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->currentGame = new Game();

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 1,4 }, { 3, 4 }, &enPassant, &castling, &promotion);//moving black pawn
	testEngine->makeTheMove({ 6,7 }, { 4, 7 }, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 0,5 }, { 4, 1 }, &enPassant, &castling, &promotion);//moving black bishop
	testEngine->makeTheMove({ 6,3 }, { 4, 3 }, &enPassant, &castling, &promotion);//moving white pawn
	char testFigure = testGame->getPieceAtPosition(6, 3);

	CHECK(test_initialBoard[6][3] == testFigure);
}

TEST_CASE("Test if pawn can be promoted") {

	char test_initialBoard[8][8] =
	{
	   { EMPTY_SQUARE,  EMPTY_SQUARE,  'B',  'Q',  'K',  EMPTY_SQUARE,  'N',  'R' },
	   { 'p',  EMPTY_SQUARE,  'P',  'P',  EMPTY_SQUARE,  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, 'B', EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p' },
	   { 'p',  'p',  'p',  'p',  EMPTY_SQUARE,  'p',  'p',  EMPTY_SQUARE },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Game* testGame = new Game(test_initialBoard);
	engine* testEngine = new engine();

	Position current = { 1,0 };
	Position future = { 0,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->currentGame = testGame;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn

	bool testPromotion = promotion.applied;

	CHECK(testPromotion);
}

//Knight tests

TEST_CASE("Test if knight moves alright") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, 'n', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  EMPTY_SQUARE,  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,1 };
	Position future = { 5,2 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	char testFigure1 = testGame->getPieceAtPosition(5, 2);

	CHECK(test_initialBoard[5][2] == testFigure1);
}

TEST_CASE("Test if knight can move to an occupied square") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE,EMPTY_SQUARE , EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 7,1 };
	Position future = { 6,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white pawn

	CHECK(test == false);
}

TEST_CASE("Test if knight takes enemy figures alright") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  EMPTY_SQUARE,  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'N', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  EMPTY_SQUARE,  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 6,4 };
	Position future = { 4,4 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 0,6 }, { 2,5 }, &enPassant, &castling, &promotion);//moving black knight
	testEngine->makeTheMove({ 6,6 }, { 5,6 }, &enPassant, &castling, &promotion);//moving white pawn
	bool test = testEngine->isMoveValid({ 2,5 }, { 4,4 }, &enPassant, &castling, &promotion);//moving black knight

	CHECK(test);
}

TEST_CASE("Test if knight can move if it would cause the king to be in check") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  EMPTY_SQUARE,  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'N', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'p',  'p',  'p',  EMPTY_SQUARE,  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 6,4 };
	Position future = { 4,4 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 0,6 }, { 2,5 }, &enPassant, &castling, &promotion);//moving black knight
	testEngine->makeTheMove({ 6,6 }, { 5,6 }, &enPassant, &castling, &promotion);//moving white pawn
	bool test = testEngine->isMoveValid({ 2,5 }, { 4,6 }, &enPassant, &castling, &promotion);//moving black knight

	CHECK(test);
}

//Bishop tests

TEST_CASE("Test if bishop moves alright") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'b',  'p', 'p',  'p',  'p' },
	   { 'r',  'n',  EMPTY_SQUARE,  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,3 };//todo
	Position future = { 4,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,7 }, { 3,7 }, &enPassant, &castling, &promotion);//moving black pawn
	testGame->movePiece({ 7,2 }, { 6,3 }, &enPassant, &castling, &promotion);//moving white bishop
	char testFigure1 = testGame->getPieceAtPosition(6, 3);

	CHECK(test_initialBoard[6][3] == testFigure1);
}

TEST_CASE("Test if can bishop move to an occupied square") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,3 };
	Position future = { 4,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 1,7 }, { 3,7 }, &enPassant, &castling, &promotion);//moving black pawn
	bool test = testEngine->isMoveValid({ 7,2 }, { 6,1 }, &enPassant, &castling, &promotion);//moving white bishop

	CHECK(test == false);
}

TEST_CASE("Test if bishop can take enemy figure") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'b', EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'r',  'n',  EMPTY_SQUARE,  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,3 };
	Position future = { 4,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,6 }, { 3,6 }, &enPassant, &castling, &promotion);//moving black pawn
	testGame->movePiece({ 7,2 }, { 3 ,6 }, &enPassant, &castling, &promotion);//moving white bishop
	char testFigure1 = testGame->getPieceAtPosition(3, 6);

	CHECK(test_initialBoard[3][6] == testFigure1);
}

TEST_CASE("Test if bishop can move if it would cause the king to be in check") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  EMPTY_SQUARE,  'P',  'P',  'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, 'B', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'b',  'p', 'p',  'p',  'p' },
	   { 'r',  'n',  EMPTY_SQUARE,  'q',  'k',  'b',  'n',  'r' },
	};
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 6,3 };
	Position future = { 4,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testEngine->makeTheMove(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testEngine->makeTheMove({ 1,4 }, { 3,4 }, &enPassant, &castling, &promotion);//moving black pawn
	testEngine->makeTheMove({ 7,2 }, { 6,3 }, &enPassant, &castling, &promotion);//moving white bishop
	testEngine->makeTheMove({ 0,5 }, { 4,1 }, &enPassant, &castling, &promotion);//moving black bishop
	bool test = testEngine->isMoveValid({ 6,3 }, { 5,4 }, &enPassant, &castling, &promotion);//moving white bishop

	CHECK(test);
}

//Rook tests

TEST_CASE("Test if rook moves alright") {
	Game* testGame = new Game();

	const char test_initialBoard[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'r', EMPTY_SQUARE, EMPTY_SQUARE, 'p', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'b',  'p', 'p',  'p',  'p' },
	   { EMPTY_SQUARE,  'n',  EMPTY_SQUARE,  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,0 };//todo
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	testGame->movePiece(current, future, &enPassant, &castling, &promotion);//moving white pawn
	testGame->movePiece({ 1,7 }, { 3,7 }, &enPassant, &castling, &promotion);//moving black pawn
	testGame->movePiece({ 7,0 }, { 6,0 }, &enPassant, &castling, &promotion);//moving white bishop
	char testFigure1 = testGame->getPieceAtPosition(7, 1);

	CHECK(test_initialBoard[7][1] == testFigure1);
}

TEST_CASE("Test if rook can take enemy figures")
{
	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { EMPTY_SQUARE,  'P',  'P',  'P',  'P',  EMPTY_SQUARE,  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'r', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE,  'p',  'p',  'b',  'p', 'p',  'p',  'p' },
	   { EMPTY_SQUARE,  'n',  EMPTY_SQUARE,  'q',  'k',  'b',  'n',  'r' },
	};

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 5,0 };//todo
	Position future = { 0,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if rook can move if could would be in check")
{
	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { EMPTY_SQUARE,  'P',  'P',  'P',  'P',  EMPTY_SQUARE,  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'P' },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'r', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, 'Q', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'b',  'r', EMPTY_SQUARE,  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	Position current = { 6,4 };//todo
	Position future = { 6,5 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

//Queen tests

TEST_CASE("Test if queen moves alright vertical or horizontal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,3 };//todo
	Position future = { 5,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if queen takes enemy figures vertical or horizontal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,3 };//todo
	Position future = { 1,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if queen moves alright diagonal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  EMPTY_SQUARE,  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,3 };//todo
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if queen takes enemy figures diagonal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'P', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  EMPTY_SQUARE,  EMPTY_SQUARE,  'p', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,3 };//todo
	Position future = { 4,0 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if queen can move if king would be in check") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'B', EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  EMPTY_SQUARE,  'q',  'p', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  EMPTY_SQUARE,  'k',  'b',  'n',  'r' },
	};

	Position current = { 6,3 };//todo
	Position future = { 5,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test == true);
}

//King tests

TEST_CASE("Test if queen moves alright vertical or horizontal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  EMPTY_SQUARE, 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,4 };//todo
	Position future = { 6,4 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if king takes enemy figures vertical or horizontal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  'P', 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,4 };//todo
	Position future = { 6,4 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if queen moves alright diagonal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  EMPTY_SQUARE,  EMPTY_SQUARE, 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,4 };//todo
	Position future = { 6,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}

TEST_CASE("Test if king takes enemy figures diagonal") {

	char test_boardToLoad[8][8] =
	{
	   { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
	   { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE },
	   { 'p',  'p',  'p',  'P',  EMPTY_SQUARE, 'p',  'p',  'p' },
	   { 'p',  'n',  'p',  'q',  'k',  'b',  'n',  'r' },
	};

	Position current = { 7,4 };//todo
	Position future = { 6,3 };
	EnPassant enPassant;
	Castling castling;
	Promotion promotion;

	Game* testGame = new Game(test_boardToLoad);
	engine* testEngine = new engine();
	testEngine->currentGame = testGame;

	bool test = testEngine->isMoveValid(current, future, &enPassant, &castling, &promotion);//moving white rook

	CHECK(test);
}