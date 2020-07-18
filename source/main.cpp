#include "engine.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Unit_Tests/test_initialBoard.cpp"

int main()
{
	// uncomment to start unit tests
	//doctest::Context().run();

	engine engine;
	engine.runGame(engine);

	return 0;
}
