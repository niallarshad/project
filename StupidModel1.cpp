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
 
 
////////////////////////////////////////////////////////////
///Entrypoint of application
////////////////////////////////////////////////////////////

struct Cell {
	bool hasBug;
}

class Bug {
private:
	sf::CircleShape m_shape;
	Vector2i gridLoc;
	vector<vector<Cell>> * grid;
public:
	Bug(const sf:CircleShape &shape, vector<vector<Cell>> * grid) : m_shape(shape), grid(grid) {}
	void Move() {
		Vector2i desiredLoc(rand() % 9 - 4, rand() % 9 - 4);

		while( grid->at(desiredLoc.x).at(desiredLoc.y).hasBug )
		{
			desiredLoc = Vector2i(rand() % 9 - 4, rand() % 9 - 4);
		}
		
		gridLoc = Vector2i(desiredLoc);
	}

	void Draw(sf::RenderWindow const & w) {
		w.Draw(m_shape);
	}
}
 
int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Stupid model 1");
	const int gridSize = 100;
	const int bugCount = 100;
	const int bugVision = 4;
		 
	vector<vector<Cell>> grid;
	vector<Bug> bugs;
	
	for(auto itr = grid.begin(); itr != grid.end(); itr++)
	{
		for(auto itr2 = itr->begin(); itr2 != itr->end(); itr++)
		{
			*itr2 = Cell();
			itr2->hasBug = false();
		}
	}



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
		
		for(auto itr = bugs.begin(); itr != bugs.end(); itr++)
		{
			itr->Draw(window);
		}

		 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
	}







