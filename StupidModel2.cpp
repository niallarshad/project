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

using namespace sf;
 
 
////////////////////////////////////////////////////////////
///Entrypoint of application
////////////////////////////////////////////////////////////

struct Cell {
	bool hasBug;

	Cell() : hasBug(false) {}
};

class Bug {
private:
	//the shape to draw as the bug
	CircleShape m_shape;

	//the location of bug in grid
	Vector2i gridLoc;

	//pointer to main grid
	std::vector<std::vector<Cell> > * grid;

public:
	Bug(Vector2i loc, float radius, std::vector<std::vector<Cell> > * grid) : gridLoc(loc), m_shape(radius), grid(grid) {
		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		m_shape.setFillColor(Color::Red);
	}
	
	void Move() {

		Vector2i desiredLoc;

		do
		{
			//the location the bug wants to move to
			desiredLoc = Vector2i(rand() % 9 - 4, rand() % 9 - 4);

			desiredLoc = gridLoc - desiredLoc;

			if(desiredLoc.x < 0)
				desiredLoc.x += 100;
			else if(desiredLoc.x >= 100)
				desiredLoc.x -= 100;

			if(desiredLoc.y < 0)
				desiredLoc.y += 100;
			else if(desiredLoc.y >= 100)
				desiredLoc.y -= 100;

		} while ( grid->at(desiredLoc.x).at(desiredLoc.y).hasBug );	//while there is already a bug at the desired location

		grid->at(gridLoc.x).at(gridLoc.y).hasBug = false;
		grid->at(desiredLoc.x).at(desiredLoc.y).hasBug = true;

		//move to the desired location
		gridLoc = Vector2i(desiredLoc);

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
	}

	//tell the render window to draw the shape
	void Draw(sf::RenderWindow &w) {
		w.draw(m_shape);
	}
	
	void grow()
	{
		float size = m_shape.getRadius();
		size++;
		m_shape.setRadius(size);
		if(size == 10.0f)
		{
			m_shape.setFillColor(Color::White);
		}
		
	}
};
 
int main()
{
	srand( time(NULL) );

	// Create the main window
	RenderWindow window(sf::VideoMode(800, 600, 32), "Stupid model 2");
		 
	std::vector<std::vector<Cell> > grid;
	std::vector<Bug> bugs;

	//fill the vectors with cells
	for( int x = 0; x < GridSize; x++ ) {

		grid.push_back( std::vector<Cell>() );

		for( int y = 0; y < GridSize; y++ ) {

			grid.at( x ).push_back( Cell() );

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
		//prepare frame
		window.clear();

		//move bugs once a second
		if ( clock.getElapsedTime().asSeconds() >= 1 ) {

			for(std::vector<Bug>::iterator itr = bugs.begin(); itr != bugs.end(); itr++)
			{
				itr->Move();
				itr->grow();
			}

			//restart the clock
			clock.restart();
		}
		
		for(std::vector<Bug>::iterator itr = bugs.begin(); itr != bugs.end(); itr++)
		{
			
			itr->Draw(window);
		}

		 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
	}







