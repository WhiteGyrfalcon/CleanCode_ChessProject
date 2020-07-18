#include "includes.h"
#include "user_interface.h"

string nextMessage;

void createNextMessage(string message)
{
   nextMessage = message;
}

void appendToNextMessage(string message)
{
   nextMessage += message;
}

void clearScreen(void)
{
   system("cls");
}

void printLogo(void)
{
   cout << "    ======================================\n";
   cout << "       _____ _    _ ______  _____ _____\n";
   cout << "      / ____| |  | |  ____|/ ____/ ____|\n";
   cout << "     | |    | |__| | |__  | (___| (___ \n";
   cout << "     | |    |  __  |  __|  \\___ \\\\___ \\ \n";
   cout << "     | |____| |  | | |____ ____) |___) |\n";
   cout << "      \\_____|_|  |_|______|_____/_____/\n\n";
   cout << "    ======================================\n\n";
}

void printMenu(void)
{
   cout << "Commands: (N)ew game\t(M)ove \t(U)ndo \t(S)ave \t(L)oad \t(Q)uit \n";
}

void printMessage(void)
{
   cout << nextMessage << endl;

   nextMessage = "";
}

void printLine(int line, int colorToStart, int colorAfterStart, Game& game)
{
   int horizontalCharactersToFormSquare = 6; 
   int verticalCharactersToFormSquare = horizontalCharactersToFormSquare / 2;

   for (int subLine = 0; subLine < verticalCharactersToFormSquare; subLine++)
   {
      for (int pairsOfBlackAndWhite = 0; pairsOfBlackAndWhite < 4; pairsOfBlackAndWhite++)
      {
         for (int subColumn = 0; subColumn < horizontalCharactersToFormSquare; subColumn++)
         {       
            if (subLine == 1 && subColumn == 3)// The piece should be in the "middle" of the cell
            {
               cout << char(game.getPieceAtPosition(line, pairsOfBlackAndWhite *2) != EMPTY_SQUARE ? game.getPieceAtPosition(line, pairsOfBlackAndWhite *2) : colorToStart);
            }
            else
            {
               cout << char(colorToStart);
            }
         }

         for (int subColumn = 0; subColumn < horizontalCharactersToFormSquare; subColumn++)
         {
            if ( subLine == 1 && subColumn == 3)
            {
               cout << char(game.getPieceAtPosition(line, pairsOfBlackAndWhite *2+1) != EMPTY_SQUARE? game.getPieceAtPosition(line, pairsOfBlackAndWhite *2+1) : colorAfterStart);
            }
            else
            {
               cout << char(colorAfterStart);
            }
         }
      }

      if (1 == subLine)
      {
         cout << "   " << line +1;
      }

      cout << "\n";
   }
}

void printHistory(Game& game)
{
   if ( 0 != game.getRounds().size() )
   {
      cout << "Last moves:\n";

      int moves = game.getRounds().size();
      int movesToShow = moves >= 5 ? 5 : moves;

      string space = "";

      while(movesToShow--)
      {
         if (moves < 10 )
         {
            // Add an extra hardspace to allign the numbers that are smaller than 10
            space = " ";
         }

         cout << space << moves << " ..... " <<  game.getRounds()[moves -1].whiteMove.c_str() << " | " << game.getRounds()[moves - 1].blackMove.c_str() << "\n";
		 moves--;
      }

      cout << "\n";
   }

   cout << "Current turn: " << (game.getCurrentTurn() == WHITE_PIECE ? "WHITE (upper case)" : "BLACK (lower case)") << "\n\n";

}

void printCaptured(Game& game) 
{
	if (0 != game.getWhiteCaptured().size() || 0 != game.getBlackCaptured().size())
	{
		cout << "---------------------------------------------\n";
		cout << "WHITE captured: ";
		for (unsigned i = 0; i < game.getWhiteCaptured().size(); i++)
		{
			cout << char(game.getWhiteCaptured()[i]) << " ";
		}
		cout << "\n";
		cout << "black captured: ";
		for (unsigned i = 0; i < game.getBlackCaptured().size(); i++)
		{
			cout << char(game.getBlackCaptured()[i]) << " ";
		}
		cout << "\n";
		cout << "---------------------------------------------\n";
	}
}

void printBoard(Game& game)
{
   cout << "   A     B     C     D     E     F     G     H\n\n";

   for (int line = 7; line >= 0; line--)
   {
      if (line %2 == 0)
      {
         printLine(line, BLACK_SQUARE, WHITE_SQUARE, game);
      }

      else
      {
         printLine(line, WHITE_SQUARE, BLACK_SQUARE, game);
      }
   }
}