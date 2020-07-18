#include "includes.h"
#include "game.h"
#include "user_interface.h"

Game::Game()
{
	currentTurn = WHITE_PLAYER;
	gameFinished = false;
	undo.capturedLastMove = false;
	undo.canUndo = false;
	undo.castlingKingSideAllowed = false;
	undo.castlingQueenSideAllowed = false;
	undo.enPassant.applied = false;
	undo.castling.applied = false;


	const char initialBoard[8][8] =
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

	// Initial board settings
	memcpy(board, initialBoard, sizeof(char) * 8 * 8);

	castlingKingSideAllowed[WHITE_PLAYER] = true;
	castlingKingSideAllowed[BLACK_PLAYER] = true;
	castlingQueenSideAllowed[WHITE_PLAYER] = true;
	castlingQueenSideAllowed[BLACK_PLAYER] = true;
}

Game::Game(char _board[8][8]) 
{
	Game::Game();
	memcpy(board, _board, sizeof(char) * 8 * 8);
}

Game::~Game()
{
	whiteCaptured.clear();
	blackCaptured.clear();
	rounds.clear();
}

bool Game::isCellEmpty(char piece)
{
	return EMPTY_SQUARE == piece;
}

bool Game::isFigureOfSameColor(char piece,int color) 
{
	return color == getPieceColor(piece);
}

bool Game::isOutOfBounds(int row, int column) 
{
	return row < 0 || row > 7 || column < 0 || column > 7;
}

void Game::movePiece(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion)
{
	char piece = getPieceAtPosition(present);
	char capturedPiece = getPieceAtPosition(future);

	if (EMPTY_SQUARE != capturedPiece)
	{
		if (WHITE_PIECE == getPieceColor(capturedPiece))
		{
			whiteCaptured.push_back(capturedPiece);
		}
		else
		{
			blackCaptured.push_back(capturedPiece);
		}
		undo.capturedLastMove = true;

		// Reset undo.castling
		memset(&undo.enPassant, 0, sizeof(EnPassant));
	}
	else if (true == enPassant->applied)
	{
		char capturedEnPassant = getPieceAtPosition(enPassant->pawnCaptured.row, enPassant->pawnCaptured.column);

		if (WHITE_PIECE == getPieceColor(capturedEnPassant))
		{
			whiteCaptured.push_back(capturedEnPassant);
		}
		else
		{
			blackCaptured.push_back(capturedEnPassant);
		}

		board[enPassant->pawnCaptured.row][enPassant->pawnCaptured.column] = EMPTY_SQUARE;

		// Set Undo structure as piece was captured and "en passant" move was performed
		undo.capturedLastMove = true;
		memcpy(&undo.enPassant, enPassant, sizeof(EnPassant));
	}
	else
	{
		undo.capturedLastMove = false;

		// Reset undo.castling
		memset(&undo.enPassant, 0, sizeof(EnPassant));
	}

	board[present.row][present.column] = EMPTY_SQUARE;

	if (true == promotion->applied)
	{
		board[future.row][future.column] = promotion->after;

		// Set Undo structure as a promotion occured
		memcpy(&undo.promotion, promotion, sizeof(Promotion));
	}
	else
	{
		board[future.row][future.column] = piece;

		// Reset undo.promotion
		memset(&undo.promotion, 0, sizeof(Promotion));
	}

	if (castling->applied == true)
	{
		char piece = getPieceAtPosition(castling->rookBefore.row, castling->rookBefore.column);
		board[castling->rookBefore.row][castling->rookBefore.column] = EMPTY_SQUARE;
		board[castling->rookAfter.row][castling->rookAfter.column] = piece;

		// Write this information to the undo struct
		memcpy(&undo.castling, castling, sizeof(Castling));

		// Save the 'CastlingAllowed' information in case the move is undone
		undo.castlingKingSideAllowed = castlingKingSideAllowed[getCurrentTurn()];
		undo.castlingQueenSideAllowed = castlingQueenSideAllowed[getCurrentTurn()];
	}
	else
	{
		// Reset undo.castling
		memset(&undo.castling, 0, sizeof(Castling));
	}

	if ('K' == toupper(piece))
	{
		// After the king has moved once, no more castling allowed
		castlingKingSideAllowed[getCurrentTurn()] = false;
		castlingQueenSideAllowed[getCurrentTurn()] = false;
	}
	else if ('R' == toupper(piece))
	{
		// After the rook has moved once, no more castling allowed
		if (0 == present.column)
		{
			castlingQueenSideAllowed[getCurrentTurn()] = false;
		}
		else if (7 == present.column)
		{
			castlingKingSideAllowed[getCurrentTurn()] = false;
		}
	}

	changeTurns();

	// This move can be undone
	undo.canUndo = true;
}

void Game::undoLastMove()
{
	string lastMove = getLastMove();
	Position from;
	Position to;
	parseMove(lastMove, &from, &to);

	char piece = getPieceAtPosition(to.row, to.column);

	if (true == undo.promotion.applied)
	{
		board[from.row][from.column] = undo.promotion.before;
	}
	else
	{
		board[from.row][from.column] = piece;
	}

	changeTurns();

	if (undo.capturedLastMove)
	{
		char captured;

		if (WHITE_PLAYER == currentTurn)
		{
			captured = blackCaptured.back();
			blackCaptured.pop_back();
		}
		else
		{
			captured = whiteCaptured.back();
			whiteCaptured.pop_back();
		}

		if (undo.enPassant.applied)
		{
			board[undo.enPassant.pawnCaptured.row][undo.enPassant.pawnCaptured.column] = captured;
			board[to.row][to.column] = EMPTY_SQUARE;
		}
		else
		{
			board[to.row][to.column] = captured;
		}
	}
	else
	{
		board[to.row][to.column] = EMPTY_SQUARE;
	}

	if (undo.castling.applied)
	{
		char rook = getPieceAtPosition(undo.castling.rookAfter.row, undo.castling.rookAfter.column);

		board[undo.castling.rookAfter.row][undo.castling.rookAfter.column] = EMPTY_SQUARE;
		board[undo.castling.rookBefore.row][undo.castling.rookBefore.column] = rook;

		castlingKingSideAllowed[getCurrentTurn()] = undo.castlingKingSideAllowed;
		castlingQueenSideAllowed[getCurrentTurn()] = undo.castlingQueenSideAllowed;
	}

	// Clean undo struct
	undo.canUndo = false;
	undo.capturedLastMove = false;
	undo.enPassant.applied = false;
	undo.castling.applied = false;
	undo.promotion.applied = false;

	gameFinished = false;

	deleteLastMove();
}

bool Game::undoIsPossible()
{
	return undo.canUndo;
}

bool Game::castlingAllowed(Side side, int color)
{
	if (QUEEN_SIDE == side)
	{
		return castlingQueenSideAllowed[color];
	}
	else
	{
		return castlingKingSideAllowed[color];
	}
}

char Game::getPieceAtPosition(int row, int column)
{
	return board[row][column];
}

char Game::getPieceAtPosition(Position position)
{
	return board[position.row][position.column];
}

char Game::getPieceConsideringMove(int row, int column, IntendedMove* intendedMove)
{
	char piece;

	if (nullptr == intendedMove)
	{
		piece = getPieceAtPosition(row, column);
	}
	else
	{
		if (intendedMove->from.row == row && intendedMove->from.column == column)
		{
			piece = EMPTY_SQUARE;
		}
		else if (intendedMove->to.row == row && intendedMove->to.column == column)
		{
			piece = intendedMove->piece;
		}
		else
		{
			piece = getPieceAtPosition(row, column);
		}
	}

	return piece;
}

UnderAttack Game::isUnderAttackHorizontal(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove)
{
	// Check all the way to the right
	for (int i = column + 1; i < 8; i++)
	{
		char pieceFound = getPieceConsideringMove(row, i, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound,color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = row;
			attack.attacker[attack.numberOfAttackers - 1].position.column = i;
			attack.attacker[attack.numberOfAttackers - 1].dir = HORIZONTAL;
			break;
		}
		else
		{
			break;
		}
	}

	// Check all the way to the left
	for (int i = column - 1; i >= 0; i--)
	{
		char pieceFound = getPieceConsideringMove(row, i, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = row;
			attack.attacker[attack.numberOfAttackers - 1].position.column = i;
			attack.attacker[attack.numberOfAttackers - 1].dir = HORIZONTAL;
			break;
		}
		else
		{
			break;
		}
	}

	return attack;
}

UnderAttack Game::isUnderAttackVertical(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove)
{
	// Check all the way up
	for (int i = row + 1; i < 8; i++)
	{
		char pieceFound = getPieceConsideringMove(i, column, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = column;
			attack.attacker[attack.numberOfAttackers - 1].dir = VERTICAL;
			break;
		}
		else
		{
			break;
		}
	}

	// Check all the way down
	for (int i = row - 1; i >= 0; i--)
	{
		char pieceFound = getPieceConsideringMove(i, column, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = column;
			attack.attacker[attack.numberOfAttackers - 1].dir = VERTICAL;
			break;
		}
		else
		{
			break;
		}
	}

	return attack;
}

UnderAttack Game::isUnderAttackInLShape(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove)
{
	Position knightMoves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
									 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
	for (int i = 0; i < 8; i++)
	{
		int rowToTest = row + knightMoves[i].row;
		int columnToTest = column + knightMoves[i].column;

		if (isOutOfBounds(rowToTest, columnToTest))
		{
			continue;
		}

		char pieceFound = getPieceConsideringMove(rowToTest, columnToTest, positionIntendedMove);

		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			continue;
		}
		else if ((toupper(pieceFound) == 'N'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = rowToTest;
			attack.attacker[attack.numberOfAttackers - 1].position.column = columnToTest;
			break;
		}
	}

	return attack;
}

UnderAttack Game::isUnderAttackDiagonal(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove)
{
	// Check the diagonal up-right
	for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++)
	{
		char pieceFound = getPieceConsideringMove(i, j, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(i == row + 1) &&
			(j == column + 1) &&
			(color == WHITE_PIECE))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal up-left
	for (int i = row + 1, j = column - 1; i < 8 && j > 0; i++, j--)
	{
		char pieceFound = getPieceConsideringMove(i, j, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(i == row + 1) &&
			(j == column - 1) &&
			(color == WHITE_PIECE))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal down-right
	for (int i = row - 1, j = column + 1; i > 0 && j < 8; i--, j++)
	{
		char pieceFound = getPieceConsideringMove(i, j, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(i == row - 1) &&
			(j == column + 1) &&
			(color == BLACK_PIECE))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal down-left
	for (int i = row - 1, j = column - 1; i > 0 && j > 0; i--, j--)
	{
		char pieceFound = getPieceConsideringMove(i, j, positionIntendedMove);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(i == row - 1) &&
			(j == column - 1) &&
			(color == BLACK_PIECE))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			attack.underAttack = true;
			attack.numberOfAttackers += 1;

			attack.attacker[attack.numberOfAttackers - 1].position.row = i;
			attack.attacker[attack.numberOfAttackers - 1].position.column = j;
			attack.attacker[attack.numberOfAttackers - 1].dir = DIAGONAL;
			break;
		}
		else
		{
			break;
		}
	}

	return attack;
}

UnderAttack Game::isUnderAttack(int row, int column, int color, IntendedMove* positionIntendedMove)
{
	UnderAttack attack = { 0 };

	isUnderAttackHorizontal(row, column, color, attack, positionIntendedMove);
	isUnderAttackVertical(row, column, color, attack, positionIntendedMove);
	isUnderAttackInLShape(row, column, color, attack, positionIntendedMove);
	isUnderAttackDiagonal(row, column, color, attack, positionIntendedMove);

	return attack;
}

bool Game::isReachableHorizontal(int row, int column, int color)
{
	bool reachable = false;

	// Check all the way to the right
	for (int i = column + 1; i < 8; i++)
	{
		char pieceFound = getPieceAtPosition(row, i);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	// Check all the way to the left
	for (int i = column - 1; i >= 0; i--)
	{
		char pieceFound = getPieceAtPosition(row, i);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}
	return reachable;
}

bool Game::isReachableVertical(int row, int column, int color)
{
	bool reachable = false;

	// Check all the way up
	for (int i = row + 1; i < 8; i++)
	{
		char pieceFound = getPieceAtPosition(i, column);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(getPieceColor(pieceFound) == BLACK_PIECE) &&
			(i == row + 1))
		{
			reachable = true;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	// Check all the way down
	for (int i = row - 1; i >= 0; i--)
	{
		char pieceFound = getPieceAtPosition(i, column);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'P') &&
			(getPieceColor(pieceFound) == WHITE_PIECE) &&
			(i == row - 1))
		{
			reachable = true;
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'R'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	return reachable;
}

bool Game::isReachableDiagonal(int row, int column, int color)
{
	bool reachable = false;

	// Check the diagonal up-right
	for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++)
	{
		char pieceFound = getPieceAtPosition(i, j);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal up-left
	for (int i = row + 1, j = column - 1; i < 8 && j > 0; i++, j--)
	{
		char pieceFound = getPieceAtPosition(i, j);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal down-right
	for (int i = row - 1, j = column + 1; i > 0 && j < 8; i--, j++)
	{
		char pieceFound = getPieceAtPosition(i, j);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	// Check the diagonal down-left
	for (int i = row - 1, j = column - 1; i > 0 && j > 0; i--, j--)
	{
		char pieceFound = getPieceAtPosition(i, j);
		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			break;
		}
		else if ((toupper(pieceFound) == 'Q') ||
			(toupper(pieceFound) == 'B'))
		{
			reachable = true;
			break;
		}
		else
		{
			break;
		}
	}

	return reachable;
}

bool Game::isReachableInLShape(int row, int column, int color)
{
	bool reachable = false;

	Position knightMoves[8] = {{  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
								 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 }};
	for (int i = 0; i < 8; i++)
	{
		int rowToTest = row + knightMoves[i].row;
		int columnToTest = column + knightMoves[i].column;

		if (isOutOfBounds(rowToTest, columnToTest))
		{
			continue;
		}

		char pieceFound = getPieceAtPosition(rowToTest, columnToTest);

		if (isCellEmpty(pieceFound))
		{
			continue;
		}

		if (isFigureOfSameColor(pieceFound, color))
		{
			continue;
		}
		else if ((toupper(pieceFound) == 'N'))
		{
			reachable = true;
			break;
		}
	}

	return reachable;
}

bool Game::isReachable(int row, int column, int color)
{
	return isReachableDiagonal(row, column, color) || isReachableHorizontal(row, column, color) || isReachableInLShape(row, column, color) || isReachableVertical(row, column, color);
}

bool Game::isSquareOccupied(int row, int column)
{
	bool occupied = false;

	if (EMPTY_SQUARE != getPieceAtPosition(row, column))
	{
		occupied = true;
	}

	return occupied;
}

bool Game::isPathFree(Position startingPosition, Position finishingPosition, int direction)
{
	bool free = false;
	try
	{
		switch (direction)
		{
		case HORIZONTAL:
		{
			if (startingPosition.column == finishingPosition.column)
			{
				cout << "Error. Movement is horizontal but column is the same\n";
			}
			else if (startingPosition.column < finishingPosition.column)
			{
				free = true;

				for (int i = startingPosition.column + 1; i < finishingPosition.column; i++)
				{
					if (isSquareOccupied(startingPosition.row, i))
					{
						free = false;
						cout << "Horizontal path to the right is not clear!\n";
					}
				}
			}
			else
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = startingPosition.column - 1; i > finishingPosition.column; i--)
				{
					if (isSquareOccupied(startingPosition.row, i))
					{
						free = false;
						cout << "Horizontal path to the left is not clear!\n";
					}
				}
			}
		}
		break;

		case VERTICAL:
		{
			if (startingPosition.row == finishingPosition.row)
			{
				cout << "Error. Movement is vertical but row is the same\n";
			}
			else if (startingPosition.row < finishingPosition.row)
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = startingPosition.row + 1; i < finishingPosition.row; i++)
				{
					if (isSquareOccupied(i, startingPosition.column))
					{
						free = false;
						cout << "Vertical path up is not clear!\n";
					}
				}
			}

			else
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = startingPosition.row - 1; i > finishingPosition.row; i--)
				{
					if (isSquareOccupied(i, startingPosition.column))
					{
						free = false;
						cout << "Vertical path down is not clear!\n";
					}
				}
			}
		}
		break;

		case DIAGONAL:
		{
			if ((finishingPosition.row > startingPosition.row) && (finishingPosition.column > startingPosition.column))
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (isSquareOccupied(startingPosition.row + i, startingPosition.column + i))
					{
						free = false;
						cout << "Diagonal path up-right is not clear!\n";
					}
				}
			}
			else if ((finishingPosition.row > startingPosition.row) && (finishingPosition.column < startingPosition.column))
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (isSquareOccupied(startingPosition.row + i, startingPosition.column - i))
					{
						free = false;
						cout << "Diagonal path up-left is not clear!\n";
					}
				}
			}
			else if ((finishingPosition.row < startingPosition.row) && (finishingPosition.column > startingPosition.column))
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (isSquareOccupied(startingPosition.row - i, startingPosition.column + i))
					{
						free = false;
						cout << "Diagonal path down-right is not clear!\n";
					}
				}
			}
			else if ((finishingPosition.row < startingPosition.row) && (finishingPosition.column < startingPosition.column))
			{
				// Settting free as initially true, only inside the cases, guarantees that the path is checked
				free = true;

				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (isSquareOccupied(startingPosition.row - i, startingPosition.column - i))
					{
						free = false;
						cout << "Diagonal path down-left is not clear!\n";
					}
				}
			}
			else
			{
				throw("Error. Diagonal move not allowed");
			}
		}
		break;
		}
	}
	catch (std::exception& ex) 
	{
		cout << "Something went wrong: " << ex.what() << endl;
	}

	return free;
}

bool Game::canBeBlocked(Position startingPosition, Position finishingPosition, int direction)
{
	bool isBlocked = false;

	try
	{
		switch (direction)
		{
		case HORIZONTAL:
		{
			if (startingPosition.column == finishingPosition.column)
			{
				cout << "Error. Movement is horizontal but column is the same\n";
			}
			else if (startingPosition.column < finishingPosition.column)
			{
				for (int i = startingPosition.column + 1; i < finishingPosition.column; i++)
				{
					if (true == isReachable(startingPosition.row, i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else
			{
				for (int i = startingPosition.column - 1; i > finishingPosition.column; i--)
				{
					if (true == isReachable(startingPosition.row, i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
		}
		break;

		case VERTICAL:
		{

			if (startingPosition.row == finishingPosition.row)
			{
				cout << "Error. Movement is vertical but row is the same\n";
			}
			else if (startingPosition.row < finishingPosition.row)
			{
				for (int i = startingPosition.row + 1; i < finishingPosition.row; i++)
				{
					if (true == isReachable(i, startingPosition.column, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else
			{
				for (int i = startingPosition.row - 1; i > finishingPosition.row; i--)
				{
					if (true == isReachable(i, startingPosition.column, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
		}
		break;

		case DIAGONAL:
		{
			if ((finishingPosition.row > startingPosition.row) && (finishingPosition.column > startingPosition.column))
			{
				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (true == isReachable(startingPosition.row + i, startingPosition.column + i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else if ((finishingPosition.row > startingPosition.row) && (finishingPosition.column < startingPosition.column))
			{
				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (true == isReachable(startingPosition.row + i, startingPosition.column - i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else if ((finishingPosition.row < startingPosition.row) && (finishingPosition.column > startingPosition.column))
			{
				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (true == isReachable(startingPosition.row - i, startingPosition.column + i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else if ((finishingPosition.row < startingPosition.row) && (finishingPosition.column < startingPosition.column))
			{
				for (int i = 1; i < abs(finishingPosition.row - startingPosition.row); i++)
				{
					if (true == isReachable(startingPosition.row - i, startingPosition.column - i, getOpponentColor()))
					{
						isBlocked = true;
					}
				}
			}
			else
			{
				throw("Error. Diagonal move not allowed");
			}
		}
		break;
		}
	}
	catch (std::exception & ex)
	{
		cout << "Something went wrong: " << ex.what() << endl;
	}

	return isBlocked;
}

bool Game::isCheckMate()
{
	bool checkmate = false;

	if (false == playerKingInCheck())
	{
		return false;
	}

	Position kingMoves[8] = {{ 1, -1 },{  1, 0 },{  1,  1 }, { 0,  1 },
									{ -1, 1 },{ -1, 0 },{ -1, -1 }, { 0, -1 }};

	Position king = findKing(getCurrentTurn());

	for (int i = 0; i < 8; i++)
	{
		int rowToTest = king.row + kingMoves[i].row;
		int columnToTest = king.column + kingMoves[i].column;

		if (isOutOfBounds(rowToTest, columnToTest))
		{
			continue;
		}
		if (EMPTY_SQUARE != getPieceAtPosition(rowToTest, columnToTest))
		{
			continue;
		}

		IntendedMove intendedMove;
		intendedMove.piece = getPieceAtPosition(king.row, king.column);
		intendedMove.from.row = king.row;
		intendedMove.from.column = king.column;
		intendedMove.to.row = rowToTest;
		intendedMove.to.column = columnToTest;

		UnderAttack kingMoved = isUnderAttack(rowToTest, columnToTest, getCurrentTurn(), &intendedMove);

		if (false == kingMoved.underAttack)
		{
			return false;
		}
	}

	UnderAttack kingAttacked = isUnderAttack(king.row, king.column, getCurrentTurn());
	if (1 == kingAttacked.numberOfAttackers)
	{
		UnderAttack kingAttacker = { 0 };
		kingAttacker = isUnderAttack(kingAttacked.attacker[0].position.row, kingAttacked.attacker[0].position.column, getOpponentColor());

		if (true == kingAttacker.underAttack)
		{
			return false;
		}
		else
		{
			char attacker = getPieceAtPosition(kingAttacked.attacker[0].position.row, kingAttacked.attacker[0].position.column);
			try
			{
				switch (toupper(attacker))
				{
				case 'P':
				case 'N':
				{
					checkmate = true;
				}
				break;

				case 'B':
				{
					if (false == canBeBlocked(kingAttacked.attacker[0].position, king, DIAGONAL))
					{
						checkmate = true;
					}
				}
				break;

				case 'R':
				{
					if (false == canBeBlocked(kingAttacked.attacker[0].position, king, kingAttacked.attacker[0].dir))
					{
						checkmate = true;
					}
				}
				break;

				case 'Q':
				{
					if (false == canBeBlocked(kingAttacked.attacker[0].position, king, kingAttacked.attacker[0].dir))
					{
						checkmate = true;
					}
				}
				break;

				default:
				{
					throw("!!!!Should not reach here. Invalid piece");
				}
				break;
				}
			}
			catch (std::exception & ex)
			{
				cout << "Something went wrong: " << ex.what() << endl;
			}
		}
	}
	else
	{
		checkmate = true;
	}

	gameFinished = checkmate;
	return checkmate;
}

bool Game::isKingInCheck(int color, IntendedMove* positionIntendedMove)
{
	bool check = false;

	Position king = { 0 };

	// Must check if the intended move is to move the king itself
	if (nullptr != positionIntendedMove && 'K' == toupper(positionIntendedMove->piece))
	{
		king.row = positionIntendedMove->to.row;
		king.column = positionIntendedMove->to.column;
	}
	else
	{
		king = findKing(color);
	}

	UnderAttack kingAttacked = isUnderAttack(king.row, king.column, color, positionIntendedMove);

	if (true == kingAttacked.underAttack)
	{
		check = true;
	}

	return check;
}

bool Game::playerKingInCheck(IntendedMove* intendedMove)
{
	return isKingInCheck(getCurrentTurn(), intendedMove);
}

bool Game::wouldKingBeInCheck(char piece, Position present, Position future, EnPassant* enPassant)
{
	IntendedMove intendedMove;

	intendedMove.piece = piece;
	intendedMove.from.row = present.row;
	intendedMove.from.column = present.column;
	intendedMove.to.row = future.row;
	intendedMove.to.column = future.column;

	return playerKingInCheck(&intendedMove);
}

Position Game::findKing(int color)
{
	char toLookFor = (WHITE_PIECE == color) ? 'K' : 'k';
	Position king = { 0 };

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			if (toLookFor == getPieceAtPosition(row, column))
			{
				king.row = row;
				king.column = column;
			}
		}
	}

	return king;
}

void Game::changeTurns(void)
{
	if (WHITE_PLAYER == currentTurn)
	{
		currentTurn = BLACK_PLAYER;
	}
	else
	{
		currentTurn = WHITE_PLAYER;
	}
}

bool Game::isFinished(void)
{
	return gameFinished;
}

int Game::getCurrentTurn(void)
{
	return currentTurn;
}

int Game::getOpponentColor(void)
{
	int color;

	if (WHITE_PLAYER == getCurrentTurn())
	{
		color = BLACK_PLAYER;
	}
	else
	{
		color = WHITE_PLAYER;
	}

	return color;
}

void Game::parseMove(string move, Position* positionFrom, Position* positionTo, char* promoted)
{
	positionFrom->column = move[0];
	positionFrom->row = move[1];
	positionTo->column = move[3];
	positionTo->row = move[4];

	// Convert columns from ['A'-'H'] to [0x00-0x07]
	positionFrom->column = positionFrom->column - 'A';
	positionTo->column = positionTo->column - 'A';

	// Convert row from ['1'-'8'] to [0x00-0x07]
	positionFrom->row = positionFrom->row - '1';
	positionTo->row = positionTo->row - '1';

	if (promoted != nullptr)
	{
		if (move[5] == '=')
		{
			*promoted = move[6];
		}
		else
		{
			*promoted = EMPTY_SQUARE;
		}
	}
}

void Game::logMove(std::string& toRecord)
{
	// If record contains only 5 chracters, add spaces
	// Because when 
	if (toRecord.length() == 5)
	{
		toRecord += "  ";
	}

	if (WHITE_PLAYER == getCurrentTurn())
	{
		Round round;
		round.whiteMove = toRecord;
		round.blackMove = "";
		rounds.push_back(round);
	}
	else
	{
		Round round = rounds[rounds.size() - 1];
		round.blackMove = toRecord;
		rounds.pop_back();
		rounds.push_back(round);
	}
}

string Game::getLastMove(void)
{
	string lastMove;

	if (BLACK_PLAYER == getCurrentTurn())
	{
		lastMove = rounds[rounds.size() - 1].whiteMove;
	}
	else
	{
		lastMove = rounds[rounds.size() - 1].blackMove;
	}

	return lastMove;
}

void Game::deleteLastMove(void)
{
	if (WHITE_PLAYER == getCurrentTurn())
	{
		rounds.pop_back();
	}
	else
	{
		Round round = rounds[rounds.size() - 1];
		round.blackMove = "";
		rounds.pop_back();
		rounds.push_back(round);
	}
}

std::deque<Round> Game::getRounds()
{
	return this->rounds;
}

std::vector<char> Game::getWhiteCaptured()
{
	return this->whiteCaptured;
}

std::vector<char> Game::getBlackCaptured()
{
	return this->blackCaptured;
}
