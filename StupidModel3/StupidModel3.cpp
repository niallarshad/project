////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#define GridSize 100
#define BugCount 100
#define BugVision 4
#define GridSpacing 5

#include "Bug.h"

using namespace sf;
 

int main()
{
	srand( time(NULL) );

	// Create the main window
	RenderWindow window(sf::VideoMode(800, 600, 32), "Stupid model 3");
		 
	std::vector<std::vector<HabitatCell> > grid;
	std::vector<Bug> bugs;

	//fill the vectors with cells
	for( int x = 0; x < GridSize; x++ ) {

		grid.push_back( std::vector<HabitatCell>() );

		for( int y = 0; y < GridSize; y++ ) {

			grid.at( x ).push_back( HabitatCell() );

		}
	}

	//place bugs randomly in the grid
	for (int i = 0; i < BugCount; i++) {
		int x = rand() % GridSize,
			y = rand() % GridSize;

		grid[x][y].hasBug = true;
		bugs.push_back( Bug(Vector2i(x,y), 1, &grid) );
	}

	Clock clock = Clock();

	while (window.isOpen())
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
		// Close window : exit
		if (Event.type == sf::Event::Closed)
		window.close();
		// Escape key : exit
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
		window.close();
		}


		//once a second
		if ( clock.getElapsedTime().asSeconds() >= 0.25f ) {

			//tell all the HabitatCells to produce food
			for(std::vector<std::vector<HabitatCell>>::iterator itr = grid.begin();
				itr != grid.end();
				itr++)
			{
				for(std::vector<HabitatCell>::iterator itr2 = itr->begin();
				itr2 != itr->end();
				itr2++)
				{
					itr2->ProduceFood();
				}
			}

			//tell all the bugs to move and grow
			for(std::vector<Bug>::iterator itr = bugs.begin(); itr != bugs.end(); itr++)
			{
				itr->Move();
				itr->Grow();
			}

			//restart the clock
			clock.restart();
		}

		//prepare frame
		window.clear();
		
		for(std::vector<Bug>::iterator itr = bugs.begin(); itr != bugs.end(); itr++)
		{
			itr->Draw(window);
		}

		window.display();

	} //loop back for next frame

	return EXIT_SUCCESS;
}







