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

	bool thereWasAnError = false;

	//uncomment when testing and enter the same filename in the following variable
	/*string fileName = "";
	fileName += ".dat";	
	testEngine->saveGame();
	try 
	{
		std::ifstream inputFile(fileName);
		std::string line;
		std::getline(inputFile, line);
		if (line == "") {
			throw("not really saved!");
		}
	}
	catch (exception ex)
	{
		thereWasAnError = true;
	}*/

	CHECK(!thereWasAnError);
}