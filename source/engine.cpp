#include "engine.h"

bool engine::isMoveValid(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion)
{
	bool valid = false;
	char piece = currentGame->getPieceAtPosition(present.row, present.column);

	switch (toupper(piece))
	{
	case 'P':
	{
		if (future.column == present.column)
		{
			// Simple move forward
			if ((Chess::isWhitePiece(piece) && future.row == present.row + 1) ||
				(Chess::isBlackPiece(piece) && future.row == present.row - 1))
			{
				if (EMPTY_SQUARE == currentGame->getPieceAtPosition(future.row, future.column))
				{
					valid = true;
				}
			}

			// Double move forward
			else if ((Chess::isWhitePiece(piece) && future.row == present.row + 2) ||
				(Chess::isBlackPiece(piece) && future.row == present.row - 2))
			{
				if (Chess::isWhitePiece(piece))
				{
					if (EMPTY_SQUARE == currentGame->getPieceAtPosition(future.row - 1, future.column) &&
						EMPTY_SQUARE == currentGame->getPieceAtPosition(future.row, future.column) &&
						1 == present.row)
					{
						valid = true;
					}
				}
				else
				{
					if (EMPTY_SQUARE == currentGame->getPieceAtPosition(future.row + 1, future.column) &&
						EMPTY_SQUARE == currentGame->getPieceAtPosition(future.row, future.column) &&
						6 == present.row)
					{
						valid = true;
					}
				}
			}
			else
			{
				return false;
			}
		}

		// The "en passant" move
		else if ((Chess::isWhitePiece(piece) && 4 == present.row && 5 == future.row && 1 == abs(future.column - present.column)) ||
			(Chess::isBlackPiece(piece) && 3 == present.row && 2 == future.row && 1 == abs(future.column - present.column)))
		{
			// It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
			string lastMove = currentGame->getLastMove();
			Position lastMoveFrom;
			Position lastMoveTo;
			currentGame->parseMove(lastMove, &lastMoveFrom, &lastMoveTo);

			char lastMovedPiece = currentGame->getPieceAtPosition(lastMoveTo.row, lastMoveTo.column);

			if (toupper(lastMovedPiece) != 'P')
			{
				return false;
			}

			if (2 == abs(lastMoveTo.row - lastMoveFrom.row) && 1 == abs(lastMoveFrom.column - present.column))
			{
				std::cout << "En passant move!\n";
				valid = true;

				enPassant->applied = true;
				enPassant->pawnCaptured.row = lastMoveTo.row;
				enPassant->pawnCaptured.column = lastMoveTo.column;
			}
		}

		// Wants to capture a piece
		else if (1 == abs(future.column - present.column))
		{
			if ((Chess::isWhitePiece(piece) && future.row == present.row + 1) || (Chess::isBlackPiece(piece) && future.row == present.row - 1))
			{
				if (EMPTY_SQUARE != currentGame->getPieceAtPosition(future.row, future.column))
				{
					valid = true;
					std::cout << "Pawn captured a piece!\n";
				}
			}
		}
		else
		{
			return false;
		}

		// If a pawn reaches its eight rank, it must be promoted to another piece
		if ((Chess::isWhitePiece(piece) && 7 == future.row) ||
			(Chess::isBlackPiece(piece) && 0 == future.row))
		{
			std::cout << "Pawn must be promoted!\n";
			promotion->applied = true;
		}
	}
	break;

	case 'R':
	{
		if ((future.row == present.row) && (future.column != present.column))
		{
			if (currentGame->isPathFree(present, future, HORIZONTAL))
			{
				valid = true;
			}
		}
		else if ((future.row != present.row) && (future.column == present.column))
		{
			if (currentGame->isPathFree(present, future, VERTICAL))
			{
				valid = true;
			}
		}
	}
	break;

	case 'N':
	{
		if ((2 == abs(future.row - present.row)) && (1 == abs(future.column - present.column)))
		{
			valid = true;
		}
		else if ((1 == abs(future.row - present.row)) && (2 == abs(future.column - present.column)))
		{
			valid = true;
		}
	}
	break;

	case 'B':
	{
		if (abs(future.row - present.row) == abs(future.column - present.column))
		{
			if (currentGame->isPathFree(present, future, DIAGONAL))
			{
				valid = true;
			}
		}
	}
	break;

	case 'Q':
	{
		if ((future.row == present.row) && (future.column != present.column))
		{
			if (currentGame->isPathFree(present, future, HORIZONTAL))
			{
				valid = true;
			}
		}
		else if ((future.row != present.row) && (future.column == present.column))
		{
			if (currentGame->isPathFree(present, future, VERTICAL))
			{
				valid = true;
			}
		}
		else if (abs(future.row - present.row) == abs(future.column - present.column))
		{
			if (currentGame->isPathFree(present, future, DIAGONAL))
			{
				valid = true;
			}
		}
	}
	break;

	case 'K':
	{
		if ((future.row == present.row) && (1 == abs(future.column - present.column)))
		{
			valid = true;
		}
		else if ((future.column == present.column) && (1 == abs(future.row - present.row)))
		{
			valid = true;
		}
		else if ((1 == abs(future.row - present.row)) && (1 == abs(future.column - present.column)))
		{
			valid = true;
		}

		// Castling
		else if ((future.row == present.row) && (2 == abs(future.column - present.column)))
		{
			if (true == currentGame->playerKingInCheck())
			{
				return false;
			}

			if (false == currentGame->isPathFree(present, future, HORIZONTAL))
			{
				return false;
			}

			if (future.column > present.column)
			{
				// if future.column is greather, it means king side
				if (false == currentGame->castlingAllowed(Side::KING_SIDE, Chess::getPieceColor(piece)))
				{
					createNextMessage("Castling to the king side is not allowed.\n");
					return false;
				}
				else
				{
					UnderAttack squareSkipped = currentGame->isUnderAttack(present.row, present.column + 1, currentGame->getCurrentTurn());
					if (false == squareSkipped.underAttack)
					{
						castling->applied = true;
						castling->rookBefore.row = present.row;
						castling->rookBefore.column = present.column + 3;
						castling->rookAfter.row = future.row;
						castling->rookAfter.column = present.column + 1;
						valid = true;
					}
				}
			}
			else
			{
				// if present.column is greather, it means queen side
				if (false == currentGame->castlingAllowed(Side::QUEEN_SIDE, Chess::getPieceColor(piece)))
				{
					createNextMessage("Castling to the queen side is not allowed.\n");
					return false;
				}
				else
				{
					UnderAttack squareSkipped = currentGame->isUnderAttack(present.row, present.column - 1, currentGame->getCurrentTurn());
					if (false == squareSkipped.underAttack)
					{
						castling->applied = true;
						castling->rookBefore.row = present.row;
						castling->rookBefore.column = present.column - 4;
						castling->rookAfter.row = future.row;
						castling->rookAfter.column = present.column - 1;
						valid = true;
					}
				}
			}
		}
	}
	break;

	default:
	{
		std::cout << "!!!!Should not reach here. Invalid piece: " << char(piece) << "\n\n\n";
	}
	break;
	}

	if (false == valid)
	{
		std::cout << "Piece is not allowed to move to that square\n";
		return false;
	}

	if (currentGame->isSquareOccupied(future.row, future.column))
	{
		char auxiliaryPiece = currentGame->getPieceAtPosition(future.row, future.column);
		if (Chess::getPieceColor(piece) == Chess::getPieceColor(auxiliaryPiece))
		{
			std::cout << "Position is already taken by a piece of the same color\n";
			return false;
		}
	}

	if (true == currentGame->wouldKingBeInCheck(piece, present, future, enPassant))
	{
		std::cout << "Move would put player's king in check\n";
		return false;
	}

	return valid;
}

void engine::makeTheMove(Position present, Position future, EnPassant* enPassant, Castling* castling, Promotion* promotion)
{
	char piece = currentGame->getPieceAtPosition(present.row, present.column);
	try
	{
		if (currentGame->isSquareOccupied(future.row, future.column))
		{
			char auxiliaryPiece = currentGame->getPieceAtPosition(future.row, future.column);

			if (Chess::getPieceColor(piece) != Chess::getPieceColor(auxiliaryPiece))
			{
				createNextMessage(Chess::describePiece(auxiliaryPiece) + " captured!\n");
			}
			else
			{
				throw("Error. We should not be making this move");
			}
		}
		else if (true == enPassant->applied)
		{
			createNextMessage("Pawn captured by \"en passant\" move!\n");
		}
	}
	catch (std::exception & ex)
	{
		cout << "Something went wrong: " << ex.what() << endl;
	}

	if ((true == castling->applied))
	{
		createNextMessage("Castling applied!\n");
	}

	currentGame->movePiece(present, future, enPassant, castling, promotion);
}

void engine::newGame(void)
{
	if (NULL != currentGame)
	{
		delete currentGame;
	}

	currentGame = new Game();
}

void engine::undoMove(void)
{
	if (false == currentGame->undoIsPossible())
	{
		createNextMessage("Undo is not possible now!\n");
		return;
	}

	currentGame->undoLastMove();
	createNextMessage("Last move was undone\n");
}
//todo
void engine::movePiece(void)
{
	std::string toRecord;

	cout << "Choose piece to be moved. (example: A1 or b2): ";

	std::string moveFrom;
	getline(cin, moveFrom);

	if (moveFrom.length() > 2)
	{
		createNextMessage("You should type only two characters (column and row)\n");
		return;
	}

	Position present;
	present.column = moveFrom[0];
	present.row = moveFrom[1];

	present.column = toupper(present.column);

	if (present.column < 'A' || present.column > 'H')
	{
		createNextMessage("Invalid column.\n");
		return;
	}

	if (present.row < '0' || present.row > '8')
	{
		createNextMessage("Invalid row.\n");
		return;
	}

	// Put in the string to be logged
	toRecord += present.column;
	toRecord += present.row;
	toRecord += "-";

	// Convert column from ['A'-'H'] to [0x00-0x07]
	present.column = present.column - 'A';
	// Convert row from ['1'-'8'] to [0x00-0x07]
	present.row = present.row - '1';

	char piece = currentGame->getPieceAtPosition(present.row, present.column);
	cout << "Piece is " << char(piece) << "\n";

	if (EMPTY_SQUARE == piece)
	{
		createNextMessage("You picked an EMPTY square.\n");
		return;
	}

	if (WHITE_PIECE == currentGame->getCurrentTurn())
	{
		if (false == Chess::isWhitePiece(piece))
		{
			createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
			return;
		}
	}
	else
	{
		if (false == Chess::isBlackPiece(piece))
		{
			createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
			return;
		}
	}

	cout << "Move to: ";
	std::string moveTo;
	getline(cin, moveTo);

	if (moveTo.length() > 2)
	{
		createNextMessage("You should type only two characters (column and row)\n");
		return;
	}

	Position future;
	future.column = moveTo[0];
	future.row = moveTo[1];

	future.column = toupper(future.column);

	if (future.column < 'A' || future.column > 'H')
	{
		createNextMessage("Invalid column.\n");
		return;
	}

	if (future.row < '0' || future.row > '8')
	{
		createNextMessage("Invalid row.\n");
		return;
	}

	// Put in the string to be logged
	toRecord += future.column;
	toRecord += future.row;

	// Convert columns from ['A'-'H'] to [0x00-0x07]
	future.column = future.column - 'A';
	// Convert row from ['1'-'8'] to [0x00-0x07]
	future.row = future.row - '1';

	if (future.row == present.row && future.column == present.column)
	{
		createNextMessage("[Invalid] You picked the same square!\n");
		return;
	}

	EnPassant enPassant = { 0 };
	Castling castling = { 0 };
	Promotion promotion = { 0 };

	if (false == isMoveValid(present, future, &enPassant, &castling, &promotion))
	{
		createNextMessage("[Invalid] Piece can not move to that square!\n");
		return;
	}

	if (promotion.applied == true)
	{
		cout << "Promote to (Q, R, N, B): ";
		std::string piece;
		getline(cin, piece);

		if (piece.length() > 1)
		{
			createNextMessage("You should type only one character (Q, R, N or B)\n");
			return;
		}

		char promoted = toupper(piece[0]);

		if (promoted != 'Q' && promoted != 'R' && promoted != 'N' && promoted != 'B')
		{
			createNextMessage("Invalid character.\n");
			return;
		}

		promotion.before = currentGame->getPieceAtPosition(present.row, present.column);

		if (WHITE_PLAYER == currentGame->getCurrentTurn())
		{
			promotion.after = toupper(promoted);
		}
		else
		{
			promotion.after = tolower(promoted);
		}

		toRecord += '=';
		toRecord += toupper(promoted); // always log with a capital letter
	}

	// Log the move: do it prior to making the move
	// because we need the getCurrentTurn()
	currentGame->logMove(toRecord);
	makeTheMove(present, future, &enPassant, &castling, &promotion);

	// Check if this move we just did put the oponent's king in check
	// Keep in mind that player turn has already changed
	if (true == currentGame->playerKingInCheck())
	{
		if (true == currentGame->isCheckMate())
		{
			if (WHITE_PLAYER == currentGame->getCurrentTurn())
			{
				appendToNextMessage("Checkmate! Black wins the game!\n");
			}
			else
			{
				appendToNextMessage("Checkmate! White wins the game!\n");
			}
		}
		else
		{
			if (WHITE_PLAYER == currentGame->getCurrentTurn())
			{
				appendToNextMessage("White king is in check!\n");
			}
			else
			{
				appendToNextMessage("Black king is in check!\n");
			}
		}
	}

	return;
}

void engine::saveGame(void)
{
	string fileName;
	cout << "Type file name to be saved (no extension): ";

	getline(cin, fileName);
	fileName += ".dat";

	std::ofstream outputFile(fileName);
	if (outputFile.is_open())
	{
		auto timeNow = std::chrono::system_clock::now();
		std::time_t endTime = std::chrono::system_clock::to_time_t(timeNow);
		outputFile << "[Chess console] Saved at: " << std::ctime(&endTime);

		for (unsigned i = 0; i < currentGame->getRounds().size(); i++)
		{
			outputFile << currentGame->getRounds()[i].whiteMove.c_str() << " | " << currentGame->getRounds()[i].blackMove.c_str() << "\n";
		}

		outputFile.close();
		createNextMessage("Game saved as " + fileName + "\n");
	}
	else
	{
		cout << "Error creating file! Save failed\n";
	}

	return;
}

void engine::loadGame(void)
{
	string fileName;
	cout << "Type file name to be loaded (no extension): ";

	getline(cin, fileName);
	fileName += ".dat";

	std::ifstream inputFile(fileName);

	if (inputFile)
	{
		if (NULL != currentGame)
		{
			delete currentGame;
		}

		currentGame = new Game();
		std::string line;

		while (std::getline(inputFile, line))
		{
			// Skip lines that starts with "[]"
			if (0 == line.compare(0, 1, "["))
			{
				continue;
			}

			// There might be one or two moves in the line
			string loaded_move[2];
			std::size_t separator = line.find(" |");
			loaded_move[0] = line.substr(0, separator);
			loaded_move[1] = line.substr(line.find("|") + 2);

			for (int i = 0; i < 2 && loaded_move[i] != ""; i++)
			{
				Position from;
				Position to;
				char promoted = 0;
				currentGame->parseMove(loaded_move[i], &from, &to, &promoted);

				if (from.column < 0 || from.column > 7 ||
					from.row < 0 || from.row > 7 ||
					to.column < 0 || to.column > 7 ||
					to.row < 0 || to.row > 7)
				{
					createNextMessage("[Invalid] Can't load this game because there are invalid lines!\n");
					currentGame = new Game();
					return;
				}

				EnPassant enPassant = { 0 };
				Castling castling = { 0 };
				Promotion promotion = { 0 };

				if (false == isMoveValid(from, to, &enPassant, &castling, &promotion))
				{
					createNextMessage("[Invalid] Can't load this game because there are invalid moves!\n");
					currentGame = new Game();
					return;
				}

				if (promotion.applied == true)
				{
					if (promoted != 'Q' && promoted != 'R' && promoted != 'N' && promoted != 'B')
					{
						createNextMessage("[Invalid] Can't load this game because there is an invalid promotion!\n");
						currentGame = new Game();
						return;
					}

					promotion.before = currentGame->getPieceAtPosition(from.row, from.column);

					if (WHITE_PLAYER == currentGame->getCurrentTurn())
					{
						promotion.after = toupper(promoted);
					}
					else
					{
						promotion.after = tolower(promoted);
					}
				}

				currentGame->logMove(loaded_move[i]);
				makeTheMove(from, to, &enPassant, &castling, &promotion);
			}
		}

		createNextMessage("Game loaded from " + fileName + "\n");
		return;
	}
	else
	{
		createNextMessage("Error loading " + fileName + ". Creating a new game instead\n");
		currentGame = new Game();
		return;
	}
}

void engine::runGame(engine engine) 
{
	bool run = true;

	clearScreen();
	printLogo();

	string input = "";

	while (run)
	{
		printMessage();
		printMenu();

		cout << "Type here: ";
		getline(cin, input);

		if (input.length() != 1)
		{
			cout << "Invalid option. Type one letter only\n\n";
			continue;
		}

		try
		{
			switch (input[0])
			{
			case 'N':
			case 'n':
			{
				engine.newGame();
				clearScreen();
				printLogo();
				printHistory(*engine.currentGame);
				printCaptured(*engine.currentGame);
				printBoard(*engine.currentGame);
			}
			break;

			case 'M':
			case 'm':
			{
				if (NULL != engine.currentGame)
				{
					if (engine.currentGame->isFinished())
					{
						cout << "This game has already finished!\n";
					}
					else
					{
						engine.movePiece();
						printLogo();
						printHistory(*engine.currentGame);
						printCaptured(*engine.currentGame);
						printBoard(*engine.currentGame);
					}
				}
				else
				{
					cout << "No game running!\n";
				}
			}
			break;

			case 'Q':
			case 'q':
			{
				run = false;
			}
			break;

			case 'U':
			case 'u':
			{
				if (NULL != engine.currentGame)
				{
					engine.undoMove();
					printLogo();
					printHistory(*engine.currentGame);
					printCaptured(*engine.currentGame);
					printBoard(*engine.currentGame);
				}
				else
				{
					cout << "No game running\n";
				}
			}
			break;

			case 'S':
			case 's':
			{
				if (NULL != engine.currentGame)
				{
					engine.saveGame();
					clearScreen();
					printLogo();
					printHistory(*engine.currentGame);
					printCaptured(*engine.currentGame);
					printBoard(*engine.currentGame);
				}
				else
				{
					cout << "No game running\n";
				}
			}
			break;

			case 'L':
			case 'l':
			{
				engine.loadGame();
				clearScreen();
				printLogo();
				printHistory(*engine.currentGame);
				printCaptured(*engine.currentGame);
				printBoard(*engine.currentGame);
			}
			break;

			default:
			{
				cout << "Option does not exist\n\n";
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