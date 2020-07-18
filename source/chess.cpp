#include "chess.h"
#include "user_interface.h"

int Chess::getPieceColor(char piece)
{
	if (isupper(piece))
	{
		return  WHITE_PIECE;
	}
	else
	{
		return  BLACK_PIECE;
	}
}

bool Chess::isWhitePiece(char piece)
{
	return Chess::getPieceColor(piece) == WHITE_PIECE ? true : false;
}

bool Chess::isBlackPiece(char piece)
{
	return Chess::getPieceColor(piece) == BLACK_PIECE ? true : false;
}

std::string Chess::describePiece(char piece)
{
	std::string description;

	if (Chess::isWhitePiece(piece))
	{
		description += "White ";
	}
	else
	{
		description += "Black ";
	}

	switch (toupper(piece))
	{
	case 'P':
	{
		description += "pawn";
	}
	break;

	case 'N':
	{
		description += "knight";
	}
	break;

	case 'B':
	{
		description += "bishop";
	}
	break;

	case 'R':
	{
		description += "rook";
	}
	break;

	case 'Q':
	{
		description += "queen";
	}
	break;

	default:
	{
		description += "unknow piece";
	}
	break;
	}

	return description;
}