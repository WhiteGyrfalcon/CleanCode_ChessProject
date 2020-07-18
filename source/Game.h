#pragma once
#include "includes.h"
#include "chess.h"


class Game : Chess
{
public:
	Game();

	Game(char board[8][8]);

	~Game();

	bool isCellEmpty(char piece);

	bool isFigureOfSameColor(char piece, int color);

	bool isOutOfBounds(int row, int column);

	void movePiece(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion);

	void undoLastMove();

	bool undoIsPossible();

	bool castlingAllowed(Side side, int color);

	char getPieceAtPosition(int row, int column);

	char getPieceAtPosition(Position position);

	char getPieceConsideringMove(int row, int column, IntendedMove* intendedMove = nullptr);

	UnderAttack isUnderAttack(int row, int column, int color, IntendedMove* positionIntendedMove = nullptr);

	UnderAttack isUnderAttackHorizontal(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove = nullptr);

	UnderAttack isUnderAttackVertical(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove = nullptr);

	UnderAttack isUnderAttackInLShape(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove = nullptr);

	UnderAttack isUnderAttackDiagonal(int row, int column, int color, UnderAttack attack, IntendedMove* positionIntendedMove = nullptr);

	bool isReachable(int row, int column, int color);

	bool isReachableHorizontal(int row, int column, int color);

	bool isReachableVertical(int row, int column, int color);

	bool isReachableDiagonal(int row, int column, int color);

	bool isReachableInLShape(int row, int column, int color);

	bool isSquareOccupied(int row, int column);

	bool isPathFree(Position startingPosition, Position finishingPosition, int direction);

	bool canBeBlocked(Position startingPosition, Position finishinPosition, int direction);

	bool isCheckMate();

	bool isKingInCheck(int color, IntendedMove* intendedMove = nullptr);

	bool playerKingInCheck(IntendedMove* intendedMove = nullptr);

	bool wouldKingBeInCheck(char piece, Position present, Position future, EnPassant* enPassant);

	Position findKing(int color);

	void changeTurns(void);

	bool isFinished(void);

	int getCurrentTurn(void);

	int getOpponentColor(void);

	void parseMove(string move, Position* positionFrom, Position* positionTo, char* promoted = nullptr);

	void logMove(std::string& toRecord);

	string getLastMove(void);

	void deleteLastMove(void);

	std::deque<Round> getRounds();

	std::vector<char> getWhiteCaptured();

	std::vector<char> getBlackCaptured();


private:
	char board[8][8];

	std::deque<Round> rounds;
	std::vector<char> whiteCaptured;
	std::vector<char> blackCaptured;

	struct Undo
	{
		bool canUndo;
		bool capturedLastMove;

		bool castlingKingSideAllowed;
		bool castlingQueenSideAllowed;

		EnPassant enPassant;
		Castling  castling;
		Promotion promotion;
	} undo;

	bool castlingKingSideAllowed[2];
	bool castlingQueenSideAllowed[2];

	int  currentTurn;
	bool gameFinished;
};