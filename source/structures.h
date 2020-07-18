#pragma once 
#include "enums.h"

struct Position
{
	int row;
	int column;
};

struct EnPassant
{
	bool applied;
	Position pawnCaptured;
};

struct Castling
{
	bool applied;
	Position rookBefore;
	Position rookAfter;
};

struct Promotion
{
	bool applied;
	char before;
	char after;
};

struct IntendedMove
{
	char piece;
	Position from;
	Position to;
};

struct Attacker
{
	Position position;
	Direction dir;
};

struct UnderAttack
{
	bool underAttack;
	int numberOfAttackers;
	Attacker attacker[9]; //maximum theorical number of attackers
};

struct Round
{
	std::string whiteMove;
	std::string blackMove;
};