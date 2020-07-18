#pragma once
#include "includes.h"

#define WHITE_SQUARE 0xDB
#define BLACK_SQUARE 0xFF
#define EMPTY_SQUARE 0x20

class Chess
{
public:
   static int getPieceColor(char piece);

   static bool isWhitePiece(char piece);

   static bool isBlackPiece(char piece);

   static std::string describePiece(char piece);

};
